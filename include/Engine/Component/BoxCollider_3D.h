#ifndef BoxCollider_3D_H
#define BoxCollider_3D_H

#include <Engine/Component/Collider.h>
#include <glm\gtx\matrix_decompose.hpp>


class BoxCollider_3D : public Collider
{
private:
	unsigned int vaoHandle;
	float V[24 * 3];
	float N[24 * 3];
	GLuint EL[36];
	double heightExtent;
	double widthExtent;
	double depthExtent;
	glm::vec3 centre;
	glm::vec3 newMaxExtents;
	glm::vec3 newMinExtents;
	glm::vec3 oldPos;
	glm::quat oldRot = glm::quat(1,1,1,1);
	glm::vec3 oldScale;
	glm::quat tempQuat;
	bool oneTime;
	bool collidingWithImmovable = false;
	bool isCollidingX = false;
	bool isCollidingY = false;
	bool isCollidingZ = false;
public:
	BoxCollider_3D();
	~BoxCollider_3D();
	BoxCollider_3D(const BoxCollider_3D &obj);
	std::string get_Type();
	void setUpBox(glm::vec3* minValues, glm::vec3* maxValues);
	void renderBox();
	bool intersects(BoxCollider_3D &other);
	void updatePos(glm::mat4 parentMatrix);
	bool getCollisionCheck() { return collidingWithImmovable; }
	void setCollisionCheck(bool isColliding);
};
#endif