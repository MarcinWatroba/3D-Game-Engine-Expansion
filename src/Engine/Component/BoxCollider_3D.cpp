#include <Engine/Component/BoxCollider_3D.h>

BoxCollider_3D::BoxCollider_3D()
{

}
BoxCollider_3D::BoxCollider_3D(const BoxCollider_3D &obj)
{

}
BoxCollider_3D::~BoxCollider_3D()
{

}
std::string BoxCollider_3D::get_Type()
{
	return "BoxCollider_3D";
}

void BoxCollider_3D::setUpBox(glm::vec3* minValues, glm::vec3* maxValues)
{
	centre = glm::vec3(((minValues->x + maxValues->x) / 2), ((minValues->y + maxValues->y) / 2), ((minValues->z + maxValues->z) / 2));
	heightExtent = maxValues->y;
	widthExtent = maxValues->x;
	depthExtent = maxValues->z;
	newMaxExtents = glm::vec3(widthExtent, heightExtent, depthExtent);
	newMinExtents = glm::vec3(-widthExtent, -heightExtent, -depthExtent);
	oldPos = glm::vec3(0, 0, 0);
	oldRot = glm::quat(0, 0, 0, 0);
	oldScale = glm::vec3(0, 0, 0);

	oneTime = true;
}

void BoxCollider_3D::renderBox()
{

}

bool BoxCollider_3D::intersects(BoxCollider_3D &other)
{
	if ((newMaxExtents.x < other.newMaxExtents.x && other.newMinExtents.x < newMaxExtents.x) || (newMaxExtents.x < other.newMinExtents.x && other.newMaxExtents.x < newMaxExtents.x)) // MaxX is between things
	{
		isCollidingX = true;
	}
	else if ((newMinExtents.x < other.newMaxExtents.x && other.newMinExtents.x < newMinExtents.x) || (newMinExtents.x < other.newMinExtents.x && other.newMaxExtents.x < newMinExtents.x)) // MinX is between things
	{
		isCollidingX = true;
	}
	else
	{
		isCollidingX = false;
	}

	if ((newMaxExtents.y < other.newMaxExtents.y && other.newMinExtents.y < newMaxExtents.y) || (newMaxExtents.y < other.newMinExtents.y && other.newMaxExtents.y < newMaxExtents.y)) // MaxY is between things
	{
		isCollidingY = true;
	}
	else if ((newMinExtents.y < other.newMaxExtents.y && other.newMinExtents.y < newMinExtents.y) || (newMinExtents.y < other.newMinExtents.y && other.newMaxExtents.y < newMinExtents.y)) // MinY is between things
	{
		isCollidingY = true;
	}
	else
	{
		isCollidingY = false;
	}

	if ((newMaxExtents.z < other.newMaxExtents.z && other.newMinExtents.z < newMaxExtents.z) || (newMaxExtents.z < other.newMinExtents.z && other.newMaxExtents.z < newMaxExtents.z)) // MaxZ is between things
	{
		isCollidingZ = true;
	}
	else if ((newMinExtents.z < other.newMaxExtents.z && other.newMinExtents.z < newMinExtents.z) || (newMinExtents.z < other.newMinExtents.z && other.newMaxExtents.z < newMinExtents.z)) // MinZ is between things
	{
		isCollidingZ = true;
	}
	else
	{
		isCollidingZ = false;
	}

	if (isCollidingX && isCollidingY && isCollidingZ)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BoxCollider_3D::setCollisionCheck(bool isColliding)
{
	collidingWithImmovable = isColliding;
}

void BoxCollider_3D::updatePos(glm::mat4 parentMatrix)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 newRot;
	glm::decompose(parentMatrix, scale, rotation, translation, skew, perspective);
	rotation = glm::conjugate(rotation);

	if (oneTime)
	{
		oldScale = scale;
		//centre = centre * rotation;
		newMaxExtents = centre + glm::vec3(widthExtent, heightExtent, depthExtent);
		newMinExtents = centre - glm::vec3(widthExtent, heightExtent, depthExtent);
		newMaxExtents = newMaxExtents * scale;
		newMinExtents = newMinExtents * scale;
		newMaxExtents = newMaxExtents * rotation;
		newMinExtents = newMinExtents * rotation;
	}

	if (oldPos != translation)
	{
		centre += (translation - oldPos);
		newMaxExtents += translation - oldPos;
		newMinExtents += translation - oldPos;
	}



	oneTime = false;
	oldPos = translation;
	oldRot = rotation;

	renderBox();
}