#include <Engine\Mesh\Mesh_2D.h>
#include <glad/glad.h>

Mesh_2D::Mesh_2D(glm::vec2 v2_TexturePos_In, glm::vec2 v2_TextureSize_In, glm::vec2 v2_TextureLength_In, glm::vec3 rgb_Colour_In)
{
	//Create vertices
	vf_Vertices2D.push_back(Vertex_2D(-1.f, +1.f, 0.f));
	vf_Vertices2D.push_back(Vertex_2D(-1.f, -1.f, 0.f));
	vf_Vertices2D.push_back(Vertex_2D(+1.f, +1.f, 0.f));
	vf_Vertices2D.push_back(Vertex_2D(+1.f, -1.f, 0.f));

	//Create UVs
	vf_UVs2D.push_back(UV_2D(v2_TexturePos_In.x / v2_TextureSize_In.x, (v2_TexturePos_In.y + v2_TextureLength_In.y) / v2_TextureSize_In.y));
	vf_UVs2D.push_back(UV_2D(v2_TexturePos_In.x / v2_TextureSize_In.x, v2_TexturePos_In.y / v2_TextureSize_In.y));
	vf_UVs2D.push_back(UV_2D((v2_TexturePos_In.x + v2_TextureLength_In.x) / v2_TextureSize_In.x, (v2_TexturePos_In.y + v2_TextureLength_In.y) / v2_TextureSize_In.y));
	vf_UVs2D.push_back(UV_2D((v2_TexturePos_In.x + v2_TextureLength_In.x) / v2_TextureSize_In.x, v2_TexturePos_In.y / v2_TextureSize_In.y));

	//Colour
	for (int i = 0; i < 4; i++) vf_Colours.push_back(Colour(rgb_Colour_In.r, rgb_Colour_In.g, rgb_Colour_In.b));

	//Indices
	vui_Indices2D.push_back(Indice(0, 1, 2));
	vui_Indices2D.push_back(Indice(3, 2, 1));

	//It's time to create buffers here
	glGenVertexArrays(1, &ui_VAO);
	glGenBuffers(1, &ui_EBO);
	glGenBuffers(Buffer_2D::Num, ui_VBO);

	glBindVertexArray(ui_VAO);

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, ui_VBO[Buffer_2D::Vertex]);
	glBufferData(GL_ARRAY_BUFFER, vf_Vertices2D.size() * sizeof(Vertex_2D), vf_Vertices2D.data(), GL_STATIC_DRAW);

	//Vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_2D), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, ui_VBO[Buffer_2D::UV]);
	glBufferData(GL_ARRAY_BUFFER, vf_UVs2D.size() * sizeof(UV_2D), vf_UVs2D.data(), GL_STATIC_DRAW);

	//UV attributes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UV_2D), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, ui_VBO[Buffer_2D::Colour]);
	glBufferData(GL_ARRAY_BUFFER, vf_Colours.size() * sizeof(Colour), vf_Colours.data(), GL_STATIC_DRAW);

	//Colour attributes
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Colour), (GLvoid*)0);

	//Bind object buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vui_Indices2D.size() * sizeof(Indice), vui_Indices2D.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

std::string Mesh_2D::get_Type()
{
	return "Mesh_2D";
}

unsigned int Mesh_2D::get_SizeOfIndices()
{
	return vui_Indices2D.size() * 3;
}


