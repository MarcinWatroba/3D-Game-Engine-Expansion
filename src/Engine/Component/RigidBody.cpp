#include <Engine\Component\RigidBody.h>

RigidBody::RigidBody()
{}
RigidBody::~RigidBody()
{}
RigidBody::RigidBody(const RigidBody &p_NewComp_In)
{
	mass = p_NewComp_In.mass;
	moveable = p_NewComp_In.moveable;
	centreOfMass = p_NewComp_In.centreOfMass;
}
RigidBody::RigidBody(Stats * stat)
{
	mass = stat->getMass();
	moveable = stat->getGravity();
}
std::string RigidBody::get_Type()
{
	return "RigidBody";
}

void RigidBody::setValues(float baseMass, bool move, glm::vec3 position)
{
	mass = 10;
	moveable = move;
	centreOfMass = position;
}

void RigidBody::setPhysics(std::string iD, bool value)
{
	boolList[iD] = value;
}

void RigidBody::setForwardForce(float force)
{
	forward_Force = force;
}

void RigidBody::setJumpForce(float force)
{
	jumpForce = force;
}

void RigidBody::setJumpHeight(float force)
{

}

void RigidBody::setGrounded(bool isGrounded)
{
	grounded = isGrounded;
}
void RigidBody::setVelocity(float temp)
{
	velocity = temp;
}

void RigidBody::update(glm::vec3 &pos)
{
	centreOfMass = pos;
	if (grounded)
	{
		velocity = phys.eulerIntegration(forward_Force, mass, pos.z, grounded);
		jumpVelocity = 0;
	}
	else
	{
		if (!reachedPeak)
		{
			if (pos.y >= jumpHeight )
			{
				reachedPeak = true;
			}
			else
			{
			jumpVelocity = phys.eulerIntegration(jumpForce, mass, pos.y, grounded);
			}
		
		}
		else
		{
			if (pos.y <= 0.5)
			{
				jumpVelocity = 0;
				grounded = true;
				reachedPeak = false;
			}
			else
			{
				jumpVelocity = phys.eulerIntegration(0, mass, pos.y, grounded);
			}
			
			
		}
		//std::cout << jumpVelocity << std::endl;
	}
		
	//}
	//if (boolList["SUVAT"])
	//{
	//	// Apply SUVAT
	//}
	//if (boolList["Gravity"])
	//{
	//	if (!grounded && moveable)
	//	{
	//		pos = phys.applyGravity(pos, mass);
	//	}
	//}

}
