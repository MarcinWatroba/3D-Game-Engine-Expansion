#pragma once

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <iostream>
#include <Engine/App/Program_Template.h>

class Application
{
private:
	//Window
	GLFWwindow* po_Window;

	//Timers
	GLfloat f_Delta;
	GLfloat f_LastFrame;
	GLfloat f_CurrentFrame;

	//Position
	double d_MousePosX;
	double d_MousePosY;

	glm::vec2 v2_Window_Size;

	glm::vec2 v2_MousePos;

	bool b_LockedLast;

	Program_Template* po_Program;
public:
	Application() {};
	//Initializer
	Application(const char* pc_GameName_In);
	~Application();

	//Create window
	void create_Window(const unsigned short ui_Width_In, const unsigned short ui_Height_In);

	//Run the game
	void run(std::string s_AppType);

	//Create call backs ic C++ way
	void create_CallBacks();

	//These are our callbacks
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scroll_mouse_callback(GLFWwindow* window, double xoffset, double yoffset);
	glm::vec2 get_Window_Size();
};

