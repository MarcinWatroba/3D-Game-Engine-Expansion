#include <Engine/Loaders/SceneLoader.h>
#include<TinyXML2\tinyxml2.h>
#include <Engine/Loaders/Loader.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Game_Objects\GameObject_Instanced.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Component\Transform_Instanced.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Component\Respond_Movement.h>
#include <Engine\Component\BoxCollider_3D.h>
#include <Engine\Component\RigidBody.h>
#include <Engine\Component\RenderComp_Instanced.h>
#include <Engine\Lighting\Point_Light.h>
#include <Game/AIController/AIController.h>
#include <Engine\Component\Character.h>

#include <glad\glad.h>
#include <Engine\Audio\Sound.h>
#include <Engine\Loaders\PrefabLoader.h>
#include <Engine\Lighting\Light.h>

#include <iostream>

int SceneLoader::get_Count() { return count; }

SceneLoader::SceneLoader(const char* pc_FileName_In, Loader* po_Loader_In, PrefabLoader* po_PrefLoader_In, std::map<std::string, Game_Object*>& mspo_GameObjects_In)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glEnable(GL_DEPTH_TEST);

	po_SceneLoader = po_Loader_In;
	i_NumOfPointLight = 0;
	int i_Incrementor = 0;
	shadowMapDimensions = glm::vec2(1024, 1024);

	aspect = shadowMapDimensions.x / shadowMapDimensions.y;
	nearP = 1.0f;
	farP = 1000.0f;
	projection = glm::perspective(glm::radians(90.0f), aspect, nearP, farP);

	tinyxml2::XMLDocument object_File;
	object_File.LoadFile(pc_FileName_In);
	
	tinyxml2::XMLElement* body = object_File.FirstChildElement("objects");
	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Object3D"); it != nullptr; it = it->NextSiblingElement("new_Object3D"))
	{
		std::cout << "Adding new object to the scene..." << "\n";

		std::string s_ObjectName = it->Attribute("name");
		std::string s_Prefab = it->Attribute("prefab");
		glm::vec3 v3_Position = to3DVector(it->Attribute("position"));
		glm::vec3 v3_Scale = to3DVector(it->Attribute("scale"));
		glm::vec3 v3_Origin = to3DVector(it->Attribute("origin"));
		glm::quat v3_Orientation = toQuat(it->Attribute("orientation"));

		auto desired_Prefab = static_cast<GameObject_3D*>(po_PrefLoader_In->get_Prefab(s_Prefab));
		auto desired_Object = new GameObject_3D(*desired_Prefab);
		mspo_GameObjects_In.insert(std::pair<std::string, Game_Object*>(s_ObjectName, desired_Object));

		if (!desired_Prefab->get_ChildrenNames().empty())
		{
			for (unsigned int i = 0; i < desired_Prefab->get_ChildrenNames().size(); i++)
			{
				std::string s_ChildName = s_ObjectName + " Child " + std::to_string(i_Incrementor + i);

				auto prefab_Child = static_cast<GameObject_3D*>(po_PrefLoader_In->get_Prefab(desired_Prefab->get_ChildrenNames()[i]));
				mspo_GameObjects_In.insert(std::pair<std::string, GameObject_3D*>(s_ChildName, new GameObject_3D(*prefab_Child)));
				auto found_Child = mspo_GameObjects_In.find(s_ChildName)->second;
				found_Child->set_Name(s_ChildName);
				found_Child->set_ObjectID(i_Incrementor);
				found_Child->set_Tag("Object_NonSavable");
				desired_Object->add_Child(found_Child);
				desired_Object->add_ChildName(desired_Prefab->get_ChildrenNames()[i]);
			}
		}

		desired_Object->set_Name(s_ObjectName);
		desired_Object->set_Position(v3_Position);
		desired_Object->set_Scale(v3_Scale);
		desired_Object->set_Rotation(v3_Orientation);
		desired_Object->set_Origin(v3_Origin);
		desired_Object->set_ObjectID(i_Incrementor);
		i_Incrementor++;
	}
	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_ObjectParticle"); it != nullptr; it = it->NextSiblingElement("new_ObjectParticle"))
	{

		std::cout << "Adding new particle object to the scene..." << "\n";

		std::string s_ObjectName = it->Attribute("name");
		std::string s_Prefab = it->Attribute("prefab");
		glm::vec3 v3_Position = to3DVector(it->Attribute("position"));
		glm::vec3 v3_Scale = to3DVector(it->Attribute("scale"));
		glm::vec3 v3_Origin = to3DVector(it->Attribute("origin"));
		glm::quat v3_Orientation = toQuat(it->Attribute("orientation"));
		unsigned int i_maxParticles = std::atoi(it->Attribute("max_Particles"));
		float life = std::atof(it->Attribute("particle_Life"));
		glm::vec3 v3_Range = to3DVector(it->Attribute("particle_Range"));
		glm::vec3 v3_Speed = to3DVector(it->Attribute("particle_Speed"));

		glm::vec3 v3_Colour = to3DVector(it->Attribute("particle_Colour"));

		auto desired_Prefab = static_cast<GameObject_Instanced*>(po_PrefLoader_In->get_Prefab(s_Prefab));
		mspo_GameObjects_In.insert(std::pair<std::string, Game_Object*>(s_ObjectName, new GameObject_Instanced(*desired_Prefab)));
		auto desired_Object = static_cast<GameObject_Instanced*>(mspo_GameObjects_In.find(s_ObjectName)->second);

		desired_Object->set_Name(s_ObjectName);
		desired_Object->set_Position(v3_Position);
		desired_Object->set_Scale(v3_Scale);
		desired_Object->set_Rotation(v3_Orientation);
		desired_Object->set_Origin(v3_Origin);
		desired_Object->set_ObjectID(i_Incrementor);
		desired_Object->setMax(i_maxParticles);
		desired_Object->set_Range(v3_Range);
		desired_Object->set_Particle_Speed(v3_Speed);
		desired_Object->set_Colour(v3_Colour);
		desired_Object->set_Life(life);

		i_Incrementor++;
	}


	body = object_File.FirstChildElement("lights");

	i_NumOfPointLight = 0;
	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Light"); it != nullptr; it = it->NextSiblingElement("new_Light"))
	{
		std::cout << "Adding new light to the scene..." << "\n";

		std::string s_ObjectName = it->Attribute("name");
		std::string s_Prefab = it->Attribute("prefab");
		glm::vec3 v3_Position = to3DVector(it->Attribute("position"));
		std::string s_Type = it->Attribute("type");

		if (s_Type == "Point_Light")
		{
			auto desired_Prefab = dynamic_cast<Point_Light*>(po_PrefLoader_In->get_Prefab(s_Prefab));
			auto desired_Object = new Point_Light(*desired_Prefab);

			desired_Object->set_Name(s_ObjectName + std::to_string(i_NumOfPointLight));
			desired_Object->set_Position(v3_Position);
			desired_Object->set_ObjectID(i_Incrementor);
			desired_Object->set_ID(i_NumOfPointLight);

			f_pos[i_NumOfPointLight] = v3_Position;
			f_radii[i_NumOfPointLight] = desired_Object->get_Radius();
			i_NumOfPointLight++;
			i_Incrementor++;

			mspo_GameObjects_In.insert(std::pair<std::string, Game_Object*>(desired_Object->get_Name(), desired_Object));
		}
	}


	count = i_Incrementor;//???
}

void SceneLoader::identify_Component(GameObject_3D* po_GameObject_In, std::string& s_ToProcess_In)
{
	std::cout << "Component name: " << s_ToProcess_In << "\n";
	if (s_ToProcess_In == "Character_Controller") { po_GameObject_In->add_Component("Character_Controller", new AIController(po_GameObject_In)); }
	//else if (s_ToProcess_In == "Patrol_Path") po_GameObject_In->add_Component("Patrol_Path", );
	else if (s_ToProcess_In == "Respond_Movement") po_GameObject_In->add_Component("Respond_Movement", new Respond_Movement());
	else if (s_ToProcess_In == "BoxCollider_3D") po_GameObject_In->add_Component("BoxCollider_3D", new BoxCollider_3D());
	else if (s_ToProcess_In == "RigidBody") po_GameObject_In->add_Component("RigidBody", new RigidBody());
	else if (s_ToProcess_In == "Character") po_GameObject_In->add_Component("Character", new Character(po_GameObject_In->get_Tag()));
	else std::cout << "Unknown component..." << "\n"; // Else we can't find it

	s_ToProcess_In.clear();
}

void SceneLoader::identify_Component_Instanced(GameObject_Instanced* po_GameObject_In, std::string& s_ToProcess_In)
{
	if (s_ToProcess_In == "YourCompnentHere") std::cout << "Nope" << "\n";
	else std::cout << "Unknown component..." << "\n"; // Else we can't find it

	s_ToProcess_In.clear();
}

glm::vec3 SceneLoader::to3DVector(const char* pc_Vector3D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector3D_In);
	glm::vec3 v3_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector3D_In[i])
		{
		case 32: // Empty space
			//Ignore
			break;

		case 44: // Comma
			i_DataCounter++;

			switch (i_DataCounter)
			{
			case 1:
				v3_Vector.x = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;

			case 2:
				v3_Vector.y = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;
			}
			break;
			break;

		case 40: // This bracker "(" 
			//Ignore
			break;

		case 41: // This bracket ")"
			v3_Vector.z = std::strtof(s_Result.c_str(), NULL);
			break;

			//Process
		default:
			s_Result += pc_Vector3D_In[i];
		break;
		}
	}

	return glm::vec3(v3_Vector.x, v3_Vector.y, v3_Vector.z);
}

glm::vec2 SceneLoader::to2DVector(const char* pc_Vector2D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector2D_In);
	glm::vec2 v2_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector2D_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			v2_Vector.x = std::strtof(s_Result.c_str(), nullptr);
			s_Result.clear();
			break;

		case 40: // This bracker "(" 
				 //Ignore
			break;

		case 41: // This bracket ")"
			v2_Vector.y = std::strtof(s_Result.c_str(), nullptr);
			break;

			//Process
		default:
			s_Result += pc_Vector2D_In[i];
			break;
		}
	}

	return glm::vec2(v2_Vector.x, v2_Vector.y);
}

glm::quat SceneLoader::toQuat(const char* pc_Quaternion_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Quaternion_In);
	//float f_Angle;
	//glm::vec3 v3_Vector;
	glm::quat temp;
	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Quaternion_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			i_DataCounter++;

				switch (i_DataCounter)
				{
					case 1:
						temp.w = std::strtof(s_Result.c_str(), nullptr);
						s_Result.clear();
					break;

					case 2:
						temp.x = std::strtof(s_Result.c_str(), nullptr);
						s_Result.clear();
					break;

					case 3:
						temp.y = std::strtof(s_Result.c_str(), nullptr);
						s_Result.clear();
				break;
			}
			break;

		case 40: // This bracket "("
				 //Ignore
			break;

			case 41: // This bracket ")"
				temp.z = std::strtof(s_Result.c_str(), nullptr);
			break;

			//Process
		default:
			s_Result = s_Result + pc_Quaternion_In[i];
			break;
		}
	}


	return temp;
}

void SceneLoader::add_Children(std::vector<std::string>& vs_Children_In, std::string s_ToProcess_In)
{
	std::string s_Result;
	int i_Length = s_ToProcess_In.length();
	bool b_IgnoreSpaces = false;

	for (int i = 0; i < i_Length; i++)
	{
		switch (s_ToProcess_In[i])
		{
		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			vs_Children_In.push_back(s_Result);
			s_Result.clear();
			b_IgnoreSpaces = true;
			break;

		case 44:  // Comma
			vs_Children_In.push_back(s_Result);
			s_Result.clear();
			b_IgnoreSpaces = true;
			break;

		case '\n':
			break;

		case '\t':
			break;

		case 32:
			if (!b_IgnoreSpaces) s_Result = s_Result + s_ToProcess_In[i];
			break;

			//Process
		default:
			s_Result = s_Result + s_ToProcess_In[i];
			if (s_Result.length() > 1) b_IgnoreSpaces = false;

			break;
		}
	}
}

void SceneLoader::add_Components(GameObject_3D* po_GameObject_In, std::string s_ToProcess_In)
{
	std::string s_Result;
	int i_Length = s_ToProcess_In.length();

	for (int i = 0; i < i_Length; i++)
	{
		switch (s_ToProcess_In[i])
		{
		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			identify_Component(po_GameObject_In, s_Result);
			break;

		case 44:  // Comma
			//Find the right component
			identify_Component(po_GameObject_In, s_Result);
			break;

		case '\n':
			break;

		case '\t':
			break;

		case 32:
			break;

			//Process
		default:
			s_Result = s_Result + s_ToProcess_In[i];
			break;
		}
	}
}

void SceneLoader::add_Components_Instanced(GameObject_Instanced* po_GameObject_In, std::string s_ToProcess_In)
{
	std::string s_Result;
	int i_Length = s_ToProcess_In.length();

	for (int i = 0; i < i_Length; i++)
	{
		switch (s_ToProcess_In[i])
		{
		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			identify_Component_Instanced(po_GameObject_In, s_Result);
			break;

		case 44:  // Comma
				  //Find the right component
			identify_Component_Instanced(po_GameObject_In, s_Result);
			break;

		case '\n':
			break;

		case '\t':
			break;

		case 32:
			break;

			//Process
		default:
			s_Result = s_Result + s_ToProcess_In[i];
			break;
		}
	}
}
glm::vec3 SceneLoader::get_LightPosition(int i)
{
	return f_pos[i];
}

float SceneLoader::get_LightRadius(int i)
{
	return f_radii[i];
}

void SceneLoader::set_LightAmount(Shader* p_Shader_In, bool add_Light)
{
	//Send this amount of light to shader
	GLint lightLoc = glGetUniformLocation(p_Shader_In->get_Program(), "numOfLights");
	glUniform1i(lightLoc, i_NumOfPointLight += add_Light);
}

glm::uvec2 SceneLoader::setup_FBO()
{

	GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };

	unsigned int depthMapFBO;
	unsigned int depthMap;

	glGenFramebuffers(1, &depthMapFBO);


	glGenTextures(1, &depthMap);


	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			shadowMapDimensions.x, shadowMapDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Assign the depth buffer texture to texture channel 0


	// Create and set up the FBO

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is complete.\n");
	}
	else {
		printf("Framebuffer is not complete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout << depthMapFBO << std::endl;

	std::cout << depthMap << std::endl;

	return glm::uvec2(depthMapFBO, depthMap);

}

void SceneLoader::prepare_DepthCube(Shader* p_Shader_In, glm::vec3 light_Pos, glm::uvec2 ui_Depth_In, unsigned int tex_Num)
{
	//static int i = 0;

	glViewport(0, 0, shadowMapDimensions.x, shadowMapDimensions.y);
	glBindFramebuffer(GL_FRAMEBUFFER, ui_Depth_In.x);
	glClear(GL_DEPTH_BUFFER_BIT);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(projection *
		glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	GLint shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[0]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[1]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
	shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[2]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
	shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[3]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
	shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[4]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
	shadow_Matrix_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowMatrices[5]");
	glUniformMatrix4fv(shadow_Matrix_Loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));

	GLint far_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "farPlane");
	glUniform1f(far_Loc, farP);

	GLint light_Loc = glGetUniformLocation(p_Shader_In->get_Program(), "lightPos");
	glUniform3f(light_Loc, light_Pos.x, light_Pos.y, light_Pos.z);


	glActiveTexture(GL_TEXTURE2 + tex_Num);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ui_Depth_In.y);
}