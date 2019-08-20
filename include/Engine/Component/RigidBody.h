#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Engine\Physics\Physics.h>
#include <iostream>
#include <string>
#include <map>
#include <Engine/Component/Component.h>
#include <Engine\Stats\Stats.h>

class RigidBody : public Component
{
private:
	Physics phys;
	std::map<std::string, bool> boolList;
	float mass;
	bool moveable;
	glm::vec3 centreOfMass;
	float forward_Force;
	float velocity;
	float jumpForce;
	float jumpVelocity;
	float jumpHeight = 7.5f;
	float counter = 0;
	bool reachedPeak = false;
	bool grounded = true;
public:
	RigidBody();
	~RigidBody();
	RigidBody(const RigidBody &p_NewComp_In);
	RigidBody(Stats * stat);
	std::string get_Type();
	void setValues(float baseMass, bool move, glm::vec3 position);
	void setPhysics(std::string iD, bool value);
	void update(glm::vec3 &newPos);
	float get_Mass()
	{
		return mass;
	}
	bool get_Moveable()
	{
		return moveable;
	}
	float getVelocity()
	{
		return velocity;
	}
	float getJumpForce()
	{
		return jumpForce;
	}
	float getJumpVelocity()
	{
		return jumpVelocity;
	}
	bool getGrounded()
	{
		return grounded;
	}
	void setVelocity(float temp);
	void setJumpForce(float force);
	void setForwardForce(float force);
	void setJumpHeight(float force);
	void setGrounded(bool isGrounded);
};
#endif