#include <Engine\Component\Respond_Movement.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Component\Transform_3D.h>
#include <glm/gtx/norm.hpp>

Respond_Movement::Respond_Movement() {}
Respond_Movement::~Respond_Movement() {}
Respond_Movement::Respond_Movement(const Respond_Movement &p_NewComp_In) {}
std::string Respond_Movement::get_Type()
{
	return "Respond_Movement";
}

void Respond_Movement::move(GameObject_3D* po_GameObject_In, glm::vec3 v3_Direction_In, float f_Speed_In)
{
	//Create a rotation point
	glm::quat quat_RotationPoint = glm::quat(0, v3_Direction_In) * f_Speed_In;

	//Create a pure quaterion
	glm::quat quat_Pure = po_GameObject_In->get_Rotation() * quat_RotationPoint * glm::conjugate(po_GameObject_In->get_Rotation());
	po_GameObject_In->set_Position(po_GameObject_In->get_Position() + glm::vec3(quat_Pure.x, quat_Pure.y, quat_Pure.z));

	//Force update children if it's a container
	if (po_GameObject_In->is_Container()) for (auto const& pair : po_GameObject_In->get_Children()) pair.second->force_Update();
}

void Respond_Movement::moveAbsolute(GameObject_3D* po_GameObject_In, glm::vec3 v3_Direction_In, float f_Speed_In)
{
	po_GameObject_In->set_Position(po_GameObject_In->get_Position() + v3_Direction_In * f_Speed_In);

	//Force update children if it's a container
	if (po_GameObject_In->is_Container()) for (auto const& pair : po_GameObject_In->get_Children()) pair.second->force_Update();
}

void Respond_Movement::turn(GameObject_3D* po_GameObject_In, float f_Angle_In, glm::vec3 v3_TurnAxis_In)
{
	//Calculate new rotation
	glm::quat quat_Rot = glm::normalize(glm::angleAxis(glm::radians(f_Angle_In), v3_TurnAxis_In) * po_GameObject_In->get_Rotation());
	po_GameObject_In->update_Rotation(quat_Rot);

	//Force update children if it's a container
	if (po_GameObject_In->is_Container()) for (auto const& pair : po_GameObject_In->get_Children()) pair.second->force_Update();
}
#include <iostream>
bool Respond_Movement::facePoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_rotation)
{
	glm::vec3 pos = po_GameObject_In->get_Position();
	pos.y = 0;
	v3_Target_In.y = 0;
	if (pos == v3_Target_In) { return false; }
	glm::vec3 toPoint = v3_Target_In - pos;

	Transform_3D* transform = static_cast<Transform_3D*>(po_GameObject_In->get_Component("Transform_3D"));
	glm::vec3 forward = transform->get_Forward();
	glm::vec3 aim = glm::normalize(toPoint);
	//if vectors are the same then angle = 0, object is facing point
	//(without this floating point error causes the dot product to be > 1 creating a nan error)
	if (glm::length2(forward - aim) < 0.001f) { return true; }

	//dot product of the aim vector and the left direction vector gives
	//positive values for left turns and negative values for right turns
	float direction = glm::dot(aim, -transform->get_Right());

	//get the desired rotation angle using the dot product
	float angle = glm::degrees(glm::acos(glm::dot(forward, aim)));

	//angle is NaN
	if (angle != angle) {
		return true;
	}

	//rotate toward point by the given rotation amount
	if (angle > f_rotation)
	{
		if (direction < 0)
		{
			turn(po_GameObject_In, f_rotation, transform->get_Up());
		}
		else
		{
			turn(po_GameObject_In , -f_rotation, transform->get_Up());
		}
		
	}
	//rotation will overshoot angle to target, move by smaller amount to face toward point
	else
	{
		if (direction < 0) { angle = -angle; }
		turn(po_GameObject_In, -angle, transform->get_Up());
		return true;
	}

	//
	return false;
}

bool Respond_Movement::atPoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In)
{
	return withinRange(po_GameObject_In, v3_Target_In, 1.0f);
}

bool Respond_Movement::withinRange(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_range)
{
	v3_Target_In.y = po_GameObject_In->get_Position().y;
	glm::vec3 dist = v3_Target_In - po_GameObject_In->get_Position();
	//avoid square root by comparing the sqaured distances
	return (glm::length2(dist) < f_range*f_range);
}

bool Respond_Movement::moveToPoint(GameObject_3D* po_GameObject_In, glm::vec3 v3_Target_In, float f_Speed_In, float f_rotation)
{
	//at the point already
	if (atPoint(po_GameObject_In, v3_Target_In)) { return true; }

	//turn to face the point
	bool facingPoint = facePoint(po_GameObject_In, v3_Target_In, f_rotation);

	//if facing point move forward
	if (facingPoint)
	{
		glm::vec3 separation = po_GameObject_In->get_Position() - v3_Target_In;
		separation.y = 0;
		float dist = glm::length(separation);
		////move toward the point by the given move speed
		if (dist > f_Speed_In)
		{
			moveAbsolute(po_GameObject_In, static_cast<Transform_3D*>(po_GameObject_In->get_Component("Transform_3D"))->get_Forward(), f_Speed_In);
		}
		//move step would overshoot target, move exactly to point
		else
		{
			moveAbsolute(po_GameObject_In, static_cast<Transform_3D*>(po_GameObject_In->get_Component("Transform_3D"))->get_Forward(), dist);
			return true;
		}
	}
	return false;
}