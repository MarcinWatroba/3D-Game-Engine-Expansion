#include <Game\Scene\Game_Scene.h>
#include <GLFW\glfw3.h>
#include <Engine/Mesh/Mesh_3D.h>
#include <Engine/Mesh/Mesh_Instanced.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Game_Objects\GameObject_Instanced.h>
#include <Engine\Loaders\SceneLoader.h>

#include <Engine/Component/RenderComp_3D.h>
#include <Engine/Component/RenderComp_Instanced.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Component\Transform_Instanced.h>
#include <Engine\Lighting\Light.h>
#include <Game\AIController\AIController.h>
#include <Game\Misc\Bullet.h>
#include <Engine\Loaders\StatsLoader.h>

#include <iostream>

Game_Scene::Game_Scene()
{
	levelList.push_back("assets/scenes/Robot_Scene.xml");
	levelList.push_back("assets/scenes/Kitchen.xml");
}

//Initialize everything once
void Game_Scene::init()
{
	rendered = false;
	//Initialize
	lock_mouse(true);

	b_Init = false;

	//quad = new renderQuad();

	camera_3D = new Camera_3D(45.f, v2_WindowSize.x / v2_WindowSize.y, 0.1f, 1000.f);
	camera_3D->set_CameraPos(glm::vec3(0.f, -20.f, 0.f));
	
	if (firstTime)
	{	
		currentLevel = 0;
		po_StatsLoader = new StatsLoader("assets/stats.xml");
		po_PrefabLoader = new PrefabLoader("assets/Prefabs.xml", po_Loader, po_StatsLoader);
		o_SceneLoader = new SceneLoader(levelList[currentLevel].c_str(), po_Loader, po_PrefabLoader, mspo_Objects);
		for (unsigned int i = 0; i < 3; i++)
		{
			depth[i] = o_SceneLoader->setup_FBO();
		}
		firstTime = false;
	}
	//snd_Audio = ;
	// Play background Audio - loaded via XML
	po_Loader->getAudioMap().at("0")->Play();
	po_Loader->getAudioMap().at("6")->Play();

	//get player pointer
	findPlayer();
	player->turn(180, glm::vec3(0, 1, 0));

	//count enemies
	i_numEnemies = 0;
	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() == "Enemy")
		{
			i_numEnemies++;
		}
	}

	
	//add lighting
	glUseProgram(po_Loader->get_Shader("6")->get_Program());
	int num = 0;
	int posNum = 0;
	ui_light_Amount = 0;
	for (auto const& pair : mspo_Objects)
	{
		pos[posNum] = static_cast<GameObject_3D*>(pair.second)->get_Position();
		if (pair.second->get_Tag() == "Light")
		{
			ui_light_Amount++;
			num++;
		}
posNum++;
	}

	for (unsigned int i = 0; i < ui_light_Amount; i++)
	{
		light[i] = o_SceneLoader->get_LightPosition(i);
		radius[i] = o_SceneLoader->get_LightRadius(1);
	}

	glUseProgram(po_Loader->get_Shader("9")->get_Program());
	GLint hdr_Loc = glGetUniformLocation(po_Loader->get_Shader("9")->get_Program(), "hdrBuffer");
	glUniform1i(hdr_Loc, 0);

	GLint bloom_Loc = glGetUniformLocation(po_Loader->get_Shader("9")->get_Program(), "bloomBlur");
	glUniform1i(bloom_Loc, 1);


	glUseProgram(po_Loader->get_Shader("10")->get_Program());
	GLint image_Loc = glGetUniformLocation(po_Loader->get_Shader("10")->get_Program(), "image");
	glUniform1i(image_Loc, 0);

	glUseProgram(po_Loader->get_Shader("0")->get_Program());
	o_SceneLoader->set_LightAmount(po_Loader->get_Shader("0"), false);

	GLint depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[0]");
	glUniform1i(depth_Cube_Loc, 2);

	depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[1]");
	glUniform1i(depth_Cube_Loc, 3);

	depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[2]");
	glUniform1i(depth_Cube_Loc, 4);

	GLint diff_Tex_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "diffuse");
	glUniform1i(diff_Tex_Loc, 0);
	GLint spec_Tex_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "specular");
	glUniform1i(spec_Tex_Loc, 1);

	GLint far_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "farPlane");
	glUniform1f(far_Loc, 1000.0f);

	//
	b_Init = true;
}

bool Game_Scene::findPlayer()
{
	for (auto const& pair : mspo_Objects)
	{
		//player found
		if (pair.second->get_Tag() == "Player")
		{
			player = static_cast<GameObject_3D*>(pair.second);
			return true;
		}
	}
	//no player in scene
	player = nullptr;
	return false;
}

//Do something with keyboard input
void Game_Scene::keyboard_Input(GLfloat f_Delta_In, GLboolean* pab_KeyArray_In, GLboolean* pab_LockedKeys_In, int i_KeyPress)
{
	if (!b_Init) { return; }
	float f_Speed = 20 * f_Delta_In;
	float f_MagicNumber = 0.7071f;
	float moveSpeed = 1;


	if (pab_KeyArray_In[GLFW_KEY_R] && !pab_LockedKeys_In[GLFW_KEY_R])
	{
		//reload_Scene();
		load_Scene(currentLevel);
		pab_LockedKeys_In[GLFW_KEY_R] = true;
	}
	if (!pab_KeyArray_In[GLFW_KEY_R]) pab_LockedKeys_In[GLFW_KEY_R] = false;

	if (pab_KeyArray_In[GLFW_KEY_ESCAPE] && !pab_LockedKeys_In[GLFW_KEY_ESCAPE])
	{
		lock_mouse(false);
	}
	if (!pab_KeyArray_In[GLFW_KEY_ESCAPE]) pab_LockedKeys_In[GLFW_KEY_ESCAPE] = false;

	//player movement
	if (findPlayer() == false) { return; }

	if (pab_KeyArray_In[GLFW_KEY_W])
	{
		player->move(glm::vec3(0, 0, 1), moveSpeed * f_Delta_In);
		if (walkCount > walkRate)
		{
			po_Loader->getAudioMap().find("1")->second->Play();
			walkCount = 0;
		}
		walkCount++;
	}
	if (pab_KeyArray_In[GLFW_KEY_S])
	{
		player->move(glm::vec3(0, 0, 1), -moveSpeed * f_Delta_In);
		if (walkCount > walkRate)
		{
			po_Loader->getAudioMap().find("1")->second->Play();
			walkCount = 0;
		}
		walkCount++;
	}

	if (pab_KeyArray_In[GLFW_KEY_SPACE])
	{
		if(static_cast<RigidBody*>(player->get_Components().at("RigidBody"))->getGrounded())
		{
			po_Loader->getAudioMap().at("7")->Play();
		}
		static_cast<RigidBody*>(player->get_Components().at("RigidBody"))->setJumpForce(1);
		static_cast<RigidBody*>(player->get_Components().at("RigidBody"))->setGrounded(false);
		
	}


	if (pab_KeyArray_In[GLFW_KEY_A])
	{
		player->move(glm::vec3(1, 0, 0), moveSpeed * f_Delta_In);
		if (walkCount > walkRate)
		{
			po_Loader->getAudioMap().find("1")->second->Play();
			walkCount = 0;
		}
		walkCount++;
	}
	if (pab_KeyArray_In[GLFW_KEY_D])
	{
		player->move(glm::vec3(1, 0, 0), -moveSpeed * f_Delta_In);
		if (walkCount > walkRate)
		{
			po_Loader->getAudioMap().find("1")->second->Play();
			walkCount = 0;
		}
		walkCount++;
	}

}

void Game_Scene::mouse_Input(GLboolean* pab_MouseArray_In, GLboolean* pab_LockedMouse_In)

{
	if (!b_Init) { return; }

	if (findPlayer() == false) { return; }

	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_1])
	{
		player->createBullet(new Bullet("Bullet", (Mesh_3D*)po_Loader->get_Mesh("7"), player, po_Loader->get_Texture("24"), po_Loader->get_Texture("7")), po_Loader->getAudioMap().find("2")->second);
		player->setFiring(true);
		shooting = true;
	}
	if (!pab_MouseArray_In[GLFW_MOUSE_BUTTON_1])
	{
		shooting = false;
	}
}

void Game_Scene::scroll_Input(glm::vec2 v2_Scroll_In)
{
}

void Game_Scene::reload_Scene()
{
	clean_Up();
	b_Init = false;
}

//Update the scene
void Game_Scene::update_Scene(GLfloat f_Delta_In, glm::vec2 v2_MousePos_In)
{
	
	////Initialize
	if (!b_Init) init();

	if (findPlayer() == false) {
		//quit to main menu
		//std::cout << "Game Over, Loser!" << std::endl;
	}
	else
	{
		if (i_numEnemies == 0 && static_cast<Character*>(player->get_Component("Character"))->getEndLevel()) {
		if (levelList.size() == 0 || currentLevel < levelList.size()-1)
		{
			currentLevel++;
			load_Scene(currentLevel);
		}
		else
		{
			//game win
		}
		//std::cout << "Winner Winner Chicken Dinner!" << std::endl;
		}
		else
		{
			static_cast<Character*>(player->get_Component("Character"))->setEndLevel(false);
		}
	}
	

	if (b_Init)
	{
		

		//update all game objects
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "Particle")
			{
				static_cast<GameObject_Instanced*>(pair.second)->update_Particles(f_Delta_In, 1.0f, 1.0f, glm::vec3(1.0f));
				pair.second->update();
				continue;
			}
			GameObject_3D* po_GameObject = dynamic_cast<GameObject_3D*>(pair.second);

		
			//update game object
			po_GameObject->update();

			//robot arm aiming
			//if (po_GameObject->get_PrefabName() == "Robot Right Arm")
			//{
			//	po_GameObject->turn(-camera_3D->get_PitchDelta(), glm::vec3(1, 0, 0));
			//}
		
			//update game components
			Component * po_Component;
		
			//Update AI character controller
			po_Component = po_GameObject->get_Component("AI_Controller");
			if (po_Component != nullptr) {
				//cast
				AIController* po_AIController = static_cast<AIController*>(po_Component);
				//set data
				AIController_Data* po_ccd = po_AIController->AccessData();
				po_ccd->deltaTime = f_Delta_In;
				po_ccd->player = player;
				po_ccd->loader = po_Loader;
				po_ccd->character = po_GameObject;
				//update
				po_AIController->Update();
			}
		
			//Rigid Body
			po_Component = po_GameObject->get_Component("RigidBody");
			if (po_Component != nullptr) {
				//cast
				RigidBody* po_RigidBody = static_cast<RigidBody*>(po_Component);
				//update
				po_RigidBody->update(po_GameObject->get_Position());
			}
		}
		

		if (player != nullptr)
		{
			//shootBullet();
			if (!shooting)
			{
				player->resetCount();
			}
			camera_3D->set_CameraPos(-player->get_Position() - glm::vec3(0, 7.5, 0));
			camera_3D->move_Mouse(f_Delta_In, v2_MousePos_In, v2_WindowSize);
			player->turn(-camera_3D->get_YawDelta(), glm::vec3(0, 1, 0));
			player->jump(glm::vec3(0, 1, 0));
			camera_3D->update();
			camera_3D->reset();
		}

		//Check for Collisions between Game Objects
		colManage.collisionChecks(mspo_Objects);

		//delete objects that have deletion flag
		auto itr = mspo_Objects.begin();
		while (itr != mspo_Objects.end()) {
			if (itr->second->get_ToDelete()) {
				destroyGameObject(itr->second);
				itr = mspo_Objects.erase(itr++);
			}
			else { ++itr; }
		}
	}
}

//Render all scene objects
void Game_Scene::render()
{
	if (b_Init)
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glUseProgram(po_Loader->get_Shader("0")->get_Program());
		unsigned int tex_No = 0;
		unsigned int light_No = 0;

		float d[100];
		std::string b_Shadow;
		GLint b_Shadow_Loc;
		std::string ui_Shadow;
		GLint ui_Shadow_Loc;

		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			d[i] = glm::distance(camera_3D->get_CameraPos(), light[i]);
			b_Shadow = "point_Light[" + std::to_string(i) + "].casts_Shadow";
			b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
			glUniform1i(b_Shadow_Loc, false);

			ui_Shadow = "point_Light[" + std::to_string(i) + "].ui_depth_Map";
			ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
			glUniform1i(ui_Shadow_Loc, 9);
		}

		unsigned int light_Nom[3];
		light_Nom[0] = 0;
		float closest_Distance = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance)
			{
				closest_Distance = d[i];
				light_Nom[0] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[0]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);
		ui_Shadow = "point_Light[" + std::to_string(light_Nom[0]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 0);
		float closest_Distance2 = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance2 && d[i] > closest_Distance)
			{
				closest_Distance2 = d[i];
				light_Nom[1] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[1]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);

		ui_Shadow = "point_Light[" + std::to_string(light_Nom[1]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 1);
		float closest_Distance3 = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance3 && d[i] > closest_Distance2)
			{
				closest_Distance3 = d[i];
				light_Nom[2] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[2]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);


		ui_Shadow = "point_Light[" + std::to_string(light_Nom[2]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 2);

		glUseProgram(po_Loader->get_Shader("6")->get_Program());

		for (int i = 0; i < 3; i++)
		{
			unsigned no = light_Nom[i];
		
			unsigned int obj_No = 0;
		
			o_SceneLoader->prepare_DepthCube(po_Loader->get_Shader("6"), light[no], depth[i], i);
		
		
			for (auto const& pair : mspo_Objects)
			{
		
				if (glm::distance(pos[obj_No], light[no]) < (o_SceneLoader->get_LightRadius(no) * 10) || pair.second->get_Tag() == "Player" || pair.second->get_Tag() == "Enemy")
				{
					if (pair.second->get_Tag() != "Object_Lamp" &&  pair.second->get_Tag() != "Particle" &&  pair.second->get_Tag() != "skybox")
					{
		
						pair.second->renderDepth(po_Loader->get_Shader("6"));
					}
					else if (pair.second->get_Tag() == "Light")
					{
						pair.second->renderDepth(po_Loader->get_Shader("6"));
					}
				}
				obj_No++;
			}
			
		}

		glBindFramebuffer(GL_FRAMEBUFFER, quad.getFBO());
		
		glViewport(0, 0, 1080, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(po_Loader->get_Shader("0")->get_Program());
		
		camera_3D->update_Shader(po_Loader->get_Shader("0"));

		glUseProgram(po_Loader->get_Shader("2")->get_Program());

		camera_3D->update_Shader(po_Loader->get_Shader("2"));
		
		glUseProgram(po_Loader->get_Shader("7")->get_Program());
		
		camera_3D->update_Shader(po_Loader->get_Shader("7"));

		glUseProgram(po_Loader->get_Shader("8")->get_Program());
		camera_3D->update_Shader(po_Loader->get_Shader("8"));
		mspo_Objects.at("Sky")->render(po_Loader->get_Shader("8"), true);

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "Light")
			{
				glUseProgram(po_Loader->get_Shader("0")->get_Program());
				static_cast<Light*>(pair.second)->update_Shader(po_Loader->get_Shader("0"));
				
				glUseProgram(po_Loader->get_Shader("2")->get_Program());
				static_cast<Light*>(pair.second)->set_Bulb_Color(po_Loader->get_Shader("2"));
				pair.second->render(po_Loader->get_Shader("2"));
			}
			else if (pair.second->get_Tag() == "Particle")
			{
				glUseProgram(po_Loader->get_Shader("7")->get_Program());
				pair.second->render(po_Loader->get_Shader("7"));
			}
			else if (pair.second->get_Tag() == "skybox")
			{
				//glUseProgram(po_Loader->get_Shader("8")->get_Program());
				//camera_3D->update_Shader(po_Loader->get_Shader("8"));
				//pair.second->render(po_Loader->get_Shader("8"), true);
			}
			else
			{
				glUseProgram(po_Loader->get_Shader("0")->get_Program());
				pair.second->render(po_Loader->get_Shader("0"));
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glActiveTexture(GL_TEXTURE0);
		bool horizontal = true, first_iteration = true;
		int amount = 10;
		glUseProgram(po_Loader->get_Shader("10")->get_Program());
		for (unsigned int i = 0; i < amount; i++)
		{
			GLint horizon = glGetUniformLocation(po_Loader->get_Shader("10")->get_Program(), "horizontal");
			glUniform1i(horizon, horizontal);

			quad.pingPong(horizontal, first_iteration);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		// --------------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(po_Loader->get_Shader("9")->get_Program());
		quad.render();
	}
}


void Game_Scene::load_Scene(int i)
{
	b_Init = false;
	clean_Up();
	
	//Load the scene
	std::string sLevel = levelList.at(i);
    o_SceneLoader = new SceneLoader(sLevel.c_str(), po_Loader, po_PrefabLoader, mspo_Objects);

}

void Game_Scene::destroyGameObject(Game_Object* po_object)
{
	//decrement enemy count
	if (po_object->get_Tag() == "Enemy")
	{
		i_numEnemies--; 
		po_Loader->getAudioMap().at("4")->Play();
		if (i_numEnemies == 0)
		{
			po_Loader->getAudioMap().at("5")->Play();
		}
	}

	if (po_object->get_Tag() == "Player") { po_Loader->getAudioMap().at("4")->Play(); }

	//Remove all components
	for (auto const& components : po_object->get_Components())
	{
		if (components.first != "Mesh_3D" && components.first != "Mesh_Instanced") delete components.second;
	}
	po_object->get_Components().clear();

	//destroy the game object
	delete po_object;
}

void Game_Scene::clean_Up()
{
	if (!mspo_Objects.empty())
	{
		//For each object
		for (auto const& pair : mspo_Objects)
		{
			destroyGameObject(pair.second);
		}

		mspo_Objects.clear();
	}

	delete o_SceneLoader;
	delete camera_3D;
}


