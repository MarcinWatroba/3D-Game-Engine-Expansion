#include <Engine/App/Application.h>

int main()
{
	Application app;
	app.create_Window(720, 480);
	app.run("Game");
	//app.create_Window(720, 480);
	//app.run("Editor");
	return 0;
}