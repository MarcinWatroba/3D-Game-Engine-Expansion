#include <Game/Game/Game.h>
#include <Game/Scene/Game_Scene.h>
#include <Engine\Loaders\Loader.h>

#include <Engine\Audio\AudioEngine.h>

//Constructor
Game::Game()
{
	b_GameIsOver = false;
	po_Loader = new Loader();
	
	// Initialise Audio
	Audio_Engine = new AudioEngine();
}

void Game::init()
{
	po_Loader->start();
	o_State.push_State(new Game_Scene());	
}

//Process input
void Game::process_Input(GLfloat deltaTime)
{
	if (!o_State.is_Empty()) // If stack is not empty
	{
		o_State.back()->keyboard_Input(deltaTime, ab_Keys, ab_LockedKeys, i_KeyPress);
		o_State.back()->mouse_Input(ab_MousePress, ab_LockedMouse);
	}
}

//Update
void Game::update(GLfloat deltaTime, glm::vec2 mouse_Pos_In)
{
	if (o_State.is_Empty()) //If stack is not empty
	{
		b_GameIsOver = true;
	}
	else // else process it
	{
		o_State.back()->set_WindowSize(v2_WindowSize);

		if (o_State.back()->is_LoaderEmpty()) {
			o_State.back()->pass_Loader(po_Loader);
			o_State.back()->pass_Audio(&snd_Audio);
		}
		mouse_Lock(o_State.back()->is_MouseLocked());
		o_State.back()->update_Scene(deltaTime, mouse_Pos_In);
	}
}

//Draw
void Game::draw()
{
	if (!o_State.is_Empty()) o_State.back()->render();
}

//Process keys
void Game::process_Key(int key, GLboolean state)
{
	ab_Keys[key] = state;
	i_KeyPress = key;
}

void Game::process_Mouse(int button, GLboolean state)
{
	ab_MousePress[button] = state;
}

void Game::process_Scroll(glm::vec2 v2_Scroll_In)
{

}

void Game::clean_Up()
{
	po_Loader->clean_Up();
	delete po_Loader;
}