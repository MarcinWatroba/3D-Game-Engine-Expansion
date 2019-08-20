#include <Engine\App\Program_Template.h>

//Is the game over?
bool Program_Template::is_GameOver()
{
	return b_GameIsOver;
}

//Lock mouse
void Program_Template::mouse_Lock(bool b_Cond_In)
{
	b_LockMouse = b_Cond_In;
}

//Is mouse locked?
bool Program_Template::is_MouseLocked()
{
	return b_LockMouse;
}

void Program_Template::set_WindowSize(glm::vec2 v2_WindowSize_In)
{
	v2_WindowSize = v2_WindowSize_In;
}