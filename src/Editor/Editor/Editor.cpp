#include <Editor/Editor/Editor.h>
#include <Engine\Loaders\Loader.h>
#include <Editor\Scene\Scene_3D.h>
#include <Editor\Scene\Scene_2D.h>

//Constructor
Editor::Editor()
{
	b_GameIsOver = false;
	po_Loader = new Loader();
	Audio_Engine = new AudioEngine();
}

void Editor::init()
{
	po_Loader->start();
	o_State.push_State(new Scene_3D());
}

//Process input
void Editor::process_Input(GLfloat deltaTime)
{
	if (!o_State.is_Empty()) // If stack is not empty
	{
		o_State.back()->keyboard_Input(deltaTime, ab_Keys, ab_LockedKeys, i_KeyPress);
		o_State.back()->mouse_Input(ab_MousePress, ab_LockedMouse);
	}
}

//Update
void Editor::update(GLfloat deltaTime, glm::vec2 mouse_Pos_In)
{
	if (o_State.is_Empty()) //If stack is not empty
	{
		b_GameIsOver = true;
	}
	else // else process it
	{
		if (o_State.back()->is_LoaderEmpty()) {
			o_State.back()->pass_Loader(po_Loader);
			o_State.back()->pass_Audio(&snd_Audio);
		}
		o_State.back()->set_WindowSize(v2_WindowSize);
		o_State.back()->update_Scene(deltaTime, mouse_Pos_In);
		mouse_Lock(o_State.back()->is_MouseLocked());
	}
}

//Draw
void Editor::draw()
{
	if (!o_State.is_Empty()) o_State.back()->render();
}

//Process keys
void Editor::process_Key(int key, GLboolean state)
{
	ab_Keys[key] = state;
	i_KeyPress = key;
}

void Editor::process_Mouse(int button, GLboolean state)
{
	ab_MousePress[button] = state;
}

void Editor::process_Scroll(glm::vec2 v2_Scroll_In)
{
	o_State.back()->scroll_Input(v2_Scroll_In);
}

void Editor::clean_Up()
{
	po_Loader->clean_Up();
	delete po_Loader;
}