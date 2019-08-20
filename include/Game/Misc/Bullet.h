#ifndef BULLET_H
#define BULLET_H

#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Mesh\Mesh_3D.h>

class Bullet : public GameObject_3D
{
private:

public:
	Bullet() {}
	Bullet(std::string s_Name_In, Mesh_3D * p_Mesh_In, GameObject_3D * parentObj, Texture * diffuse, Texture * specular);
	~Bullet() {}

};

#endif