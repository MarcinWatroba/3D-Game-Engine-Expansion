#pragma once
#include <Engine\Component\Component.h>

class GameObject_3D;

class Respond_Movement : public Component
{
private:
	//Last Direction for Movement
	//bool lastDir = false;
public:
	Respond_Movement();
	~Respond_Movement();
	Respond_Movement(const Respond_Movement &p_NewComp_In);
	std::string get_Type();
	//move an object by the given amount along a relative direction vector (e.g vec3(0,0,1) will always be the forward vector of the object)
	void move(GameObject_3D* po_GameObject_In, glm::vec3 v3_Direction_In, float f_Speed_In);
	//move an object by the given amount along a absolute direction vector
	void moveAbsolute(GameObject_3D* po_GameObject_In, glm::vec3 v3_Direction_In, float f_Speed_In);
	//turn the object by the given angle in degrees around the given axis
	void turn(GameObject_3D* po_GameObject_In, float f_Angle_In, glm::vec3 v3_TurnAxis_In);
	//face game object towards a target point, rotating by a maximum of the given rotation amount (less if that would overshoot)
	//returns true if aiming toward the point, false otherwise
	bool facePoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_rotation);
	//return true if object is at a point, +- the given tolerance
	bool atPoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In);
	//return true if object is within the specified range from the target point
	bool withinRange(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_range);
	//rotate to face the target point and then move to the point, return true if object has reached the target point
	bool moveToPoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_Speed_In, float f_rotation);
};