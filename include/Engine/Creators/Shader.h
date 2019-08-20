#pragma once
class Shader
{
private:
	//Program ID
	int programHandle;

public:
	//Constructor
	Shader() {};

	//Main constructor
	Shader(const char* pc_VertexShader_In, const char* pc_FragmentShader_In, const char* pc_GeometryShader_In);

	//Get program
	int get_Program();
};
