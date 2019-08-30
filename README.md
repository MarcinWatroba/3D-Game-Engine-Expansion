# Basic 3D Game Engine

My own repository for expanding and improving the game engine originally built with my group as a University project. Original group project Github repository can be found <a href="https://github.com/marcin388/3D-Game-Engine">here.</a> 

The game engine supports 3D models, textures, dynamic light sources, dynamic shadows, skyboxes, particle effects, bloom, and HDR. Each light source along with shadows it casts is dynamically updated when moved in the editor.

Game engine uses Euler Method and AABB collision for physics and collision detection. AI behaviours are achieved using Finite-State Machine where Non-Player Characters can behave differently based on the current state they are in, (such as patrol and attack). GUI level editor has been implemented that allows to load scenes, as well as add, delete, move, scale, and transform objects within each scene. 

Compiler used: Microsoft Visual Studio 2015
Version: 14.0.25431.01 Update 3

<h2>Screenshot</h2>
<img src="https://github.com/marcin388/3D-Game-Engine-Expansion/blob/master/assets/textures/gameEng3.jpg">

<h2>Libraries used</h2>

1. SOIL - To load textures in readable format for OpenGL, Version: 7 July 2008

2. TinyXML2 - To parse xml files, Version: 4.0.1

3. Glfw3 - A lightweight window and input handling library, Version: 3.2.1

4. Glew32 - Library used for establishing OpenGL calls, Version: 2.0.0

<h2>How to compile the code</h2>
Use either Debug or Release configuration and select x86 (32 bit) solution.

<h2>Choosing between game/editor</h2>
To run game:
<ul>
  <li>Go into src->Engine->App->main.cpp</li>
  <li>Comment "app.run("Editor");"</li>
  <li>Uncomment "app.run("Game");"</li>
</ul>
To run editor:
<ul>
  <li>Go into src->Engine->App->main.cpp</li>
  <li>Comment "app.run("Game");"</li>
  <li>Uncomment "app.run("Editor");"</li>
</ul>
