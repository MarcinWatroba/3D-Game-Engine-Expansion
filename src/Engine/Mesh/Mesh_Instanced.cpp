#include <Engine\Mesh\Mesh_Instanced.h>
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>

Mesh_Instanced::Mesh_Instanced(const char* pc_FileName_In, int i_DrawMode_In)
{
	std::ifstream file;
	std::istringstream stream;
	std::string line;
	char token;
	file.open(pc_FileName_In);

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			stream.str(line);
			stream >> token;

			switch (token)
			{
			case 'v':

				Vertex f_Vertex;
				stream >> f_Vertex.x;
				stream >> f_Vertex.y;
				stream >> f_Vertex.z;
				vf_Vertices.push_back(f_Vertex);
				break;

			case 'n':
				Normal f_Normal;
				stream >> f_Normal.x;
				stream >> f_Normal.y;
				stream >> f_Normal.z;
				vf_Normals.push_back(f_Normal);
				break;

			case 't':
				UV f_UV;
				stream >> f_UV.u;
				stream >> f_UV.v;
				vf_UVs.push_back(f_UV);

				break;

			case 'f':
				for (int i = 0; i < 3; i++)
				{
					Face ui_Face;
					stream >> ui_Face.vertex;
					stream.get();
					stream >> ui_Face.uv;
					stream.get();
					stream >> ui_Face.normal;
					vui_Faces.push_back(ui_Face);
				}
				break;
			}
			stream.clear();
		}
		file.close();
	}
	else
	{
		std::cout << "Failed to open the file at: " << pc_FileName_In << "\n";
	}

	unsigned int i_Counter = 0;

	for (unsigned int i = 0; i < vui_Faces.size(); i++)
	{
		VertexIndex random = VertexIndex(vui_Faces.at(i).vertex - 1, vui_Faces.at(i).uv - 1, vui_Faces.at(i).normal - 1);

		auto found = moui_Indices.find(random);

		if (found == moui_Indices.end())
		{
			moui_Indices.insert(std::pair<VertexIndex, unsigned int>(random, i_Counter));

			//CompleteVertex comp_Vertex;
			//comp_Vertex.vertex = vf_Vertices.at(vui_Faces.at(i).vertex - 1);
			//comp_Vertex.uv = vf_UVs.at(vui_Faces.at(i).uv - 1);
			//comp_Vertex.normal = vf_Normals.at(vui_Faces.at(i).normal - 1);

			//vf_Data.push_back(comp_Vertex);
			vui_Indices.push_back(i_Counter);
			i_Counter++;
		}
		else
		{
			vui_Indices.push_back(found->second);
		}
	}

	unsigned int handle[3];
	//unsigned int vaos;
	//It's time to create buffers here
	glGenVertexArrays(1, &ui_VAO);
	glGenBuffers(1, &ui_EBO);
	glGenBuffers(3, handle);
	instanceBufferHandle = handle[2];
	
	glBindVertexArray(ui_VAO);
	VAO2 = ui_VAO;
	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vf_Vertices.size() * 3 * sizeof(float), vf_Vertices.data(), GL_STATIC_DRAW);

	//Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);	//binding UV buffer
	glBufferData(GL_ARRAY_BUFFER, vf_UVs.size() * 2 * sizeof(float), vf_UVs.data(), GL_STREAM_DRAW);	//specify size, data, and drawing mode
	//UV attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(1);
	int max = 100000;
	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * max, NULL, GL_STREAM_DRAW);

	//Normal attributes
	glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//Bind object buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vui_Indices.size() * sizeof(GLuint), vui_Indices.data(), GL_STATIC_DRAW);



	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);

}

std::string Mesh_Instanced::get_Type()
{
	return "Mesh_Instanced";
}
