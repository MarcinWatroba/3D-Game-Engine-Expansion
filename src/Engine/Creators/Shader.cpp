#include <Engine/Creators/Shader.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad\glad.h>

Shader::Shader(const char* pc_VertexShader_In, const char* pc_FragmentShader_In, const char* pc_GeometryShader_In)
{
	//Streams to store the whole file
	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;
	std::stringstream geometryShaderStream;

	//Status of compilation
	int compiled;

	//Sources for openGL
	const char* vertexSource;
	const char* fragmentSource;
	const char* geometrySource;

	//Files
	std::ifstream openVertex;
	std::ifstream openFragment;
	std::ifstream openGeometry;
	bool b_GeometryIn = false;
	if (pc_GeometryShader_In != nullptr && pc_GeometryShader_In[0] != '\0') b_GeometryIn = true;
	
	//Open
	openVertex.open(pc_VertexShader_In);
	openFragment.open(pc_FragmentShader_In);

	//If open
	if (openVertex.is_open() && openFragment.is_open())
	{
		//Read the buffer
		vertexShaderStream << openVertex.rdbuf();
		fragmentShaderStream << openFragment.rdbuf();

		//Close them
		openVertex.close();
		openFragment.close();
	}

	std::string vertexShaderStringSource = vertexShaderStream.str();
	std::string fragmentShaderStringSource = fragmentShaderStream.str();
	std::string geometryShaderStringSource;

	//Conver from string to C_String
	vertexSource = vertexShaderStringSource.c_str();
	fragmentSource = fragmentShaderStringSource.c_str();

	//Clear them when no longer needed
	vertexShaderStream.clear();
	fragmentShaderStream.clear();

	//Create vertex shader
	GLint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexSource, nullptr); // Point at the source
	glCompileShader(vertShader); // Compile it

	//Get any compile errors
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLength;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetShaderInfoLog(vertShader, logLength, nullptr, log);
			//std::cout << "Vertex shader compilation failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}

	//Create fragment shader
	GLint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragShader);

	//Get any compile errors
	if (!compiled)
	{
		GLint logLength;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetShaderInfoLog(fragShader, logLength, nullptr, log);
			std::cout << "Fragment shader compilation failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}


	GLint geoShader;
	if (b_GeometryIn)
	{
		openGeometry.open(pc_GeometryShader_In);

		if (openGeometry.is_open())
		{
			//Read the buffer
			geometryShaderStream << openGeometry.rdbuf();
			//Close them
			openGeometry.close();
		}

		geometryShaderStringSource = geometryShaderStream.str();
		geometrySource = geometryShaderStringSource.c_str();
		geometryShaderStream.clear();

		//Create vertex shader
		geoShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geoShader, 1, &geometrySource, nullptr); // Point at the source
		glCompileShader(geoShader); // Compile it

		//Get any compile errors
		glGetShaderiv(geoShader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint logLength;
			glGetShaderiv(geoShader, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				char* log = new char[logLength];
				glGetShaderInfoLog(vertShader, logLength, nullptr, log);
				std::cout << "Geometry shader compilation failure..." << "\n" << log << "\n";
				delete[] log;
			}
		}
	}

	//Program
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);
	if (b_GeometryIn) glAttachShader(programHandle, geoShader);
	glLinkProgram(programHandle);

	//Linking errors
	glGetProgramiv(programHandle, GL_LINK_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetProgramInfoLog(programHandle, logLength, nullptr, log);
			std::cout << "Program linking failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}

	//We are done now delete everything
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

GLint Shader::get_Program()
{
	return programHandle;
}
