#include <Engine\Physics\CollisionManager.h>


CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::collision(Game_Object* objectA, Game_Object* objectB, std::map<Game_Object*, Game_Object*>& collisions)
{
	//Collision with immov
	if (objectA->get_Name() == "Robot")
	{
 		if (!objectB->get_Components().count("RigidBody") || !objectA->get_Components().count("RigidBody"))
		{
			//no rigid body attached to one of the input game objects
			return;
		}
		//character collectables
		Character* character = dynamic_cast<Character*>(objectA->get_Component("Character"));
		if (character != nullptr)
		{
			if (objectB->get_Tag() == "AmmoPack")
			{
				character->gainBullets(30);
				objectB->set_ToDelete();
			}
			else if (objectB->get_Tag() == "HealthPack")
			{
				character->gainLife(1);
				objectB->set_ToDelete();
			}
			else if (objectB->get_Tag() == "Exit")
			{
				character->setEndLevel(true);
			}
			else if (objectB->get_Tag() == "Floor")
			{
				static_cast<RigidBody*>(objectA->get_Component("RigidBody"))->setGrounded(true);
			}
		}

			//other interactions
			RigidBody* tempBody = dynamic_cast<RigidBody*>(objectB->get_Components().at("RigidBody"));
			if (objectB->get_Tag() == "Enemy")
			{

			}
			else if (objectB->get_Tag() == "Floor")
			{
				//std::cout << "Dab" << std::endl;
				dynamic_cast<RigidBody*>(objectA->get_Components().at("RigidBody"))->setGrounded(true);
			}

			else if (!tempBody->get_Moveable())
			{
				//Stop the object moving in the current direction
				collisions.insert(std::make_pair(objectA, objectB));
			}
		}
}

void CollisionManager::collisionChecks(std::map<std::string, Game_Object*> &gameObjects)
{
	std::map<Game_Object*, Game_Object*> colChecks; // Body part of Robot, Thing it's colliding with


	//iterate over list of game objects
	for (auto const& pair : gameObjects)
	{
		//get collider if exists
		Game_Object* currentObject = pair.second;
		//skip if object is pending deletion
		if (currentObject->get_ToDelete()) { continue; }
		if (currentObject->get_Tag() == "Particle") { continue; }

		if (currentObject->get_Components().count("BoxCollider_3D"))
		{
			BoxCollider_3D* firstCollider = dynamic_cast<BoxCollider_3D*>(currentObject->get_Components().at("BoxCollider_3D"));

			//for every collidable object iterate over list of objects again to check for collisions
			for (auto const& pair : gameObjects)
			{
				if (currentObject->get_Tag() == "Particle") { continue; }
				//skip if second object is same as first or if second object is pending deletion
				if (currentObject == pair.second || pair.second->get_ToDelete()) 
				{ 
					continue; 
				}

				BoxCollider_3D* secondCollider = dynamic_cast<BoxCollider_3D*>(pair.second->get_Component("BoxCollider_3D"));
				if (secondCollider != nullptr)
				{
					//check if colliding
					if (firstCollider->intersects(*secondCollider))
					{
						collision(currentObject, pair.second, colChecks);
					}
				}
			}
		}

		if (static_cast<GameObject_3D*>(currentObject)->get_BulletList().size() != 0)
		{
			for (unsigned int i = 0; i < static_cast<GameObject_3D*>(currentObject)->get_BulletList().size(); i++)
			{
				Game_Object* bullet = static_cast<GameObject_3D*>(currentObject)->get_BulletList()[i];
				if (bullet->get_Components().count("BoxCollider_3D"))//bullets always have boxcollider
				{
					BoxCollider_3D* tempCol = dynamic_cast<BoxCollider_3D*>(bullet->get_Components().at("BoxCollider_3D"));
					for (auto const& pair2 : gameObjects)
					{
					//if (currentObject == pair2.second) { continue; }
						BoxCollider_3D* secondCol = dynamic_cast<BoxCollider_3D*>(pair2.second->get_Component("BoxCollider_3D"));
						if (secondCol != nullptr)
						{
							bool check = tempCol->intersects(*secondCol);
							if (check)
							{
								//std::cout << "Toppus Kekkus" << std::endl;
								if ((pair2.second->get_Components().count("RigidBody") && bullet->get_Components().count("RigidBody")))
								{
									RigidBody* tempBody;
									tempBody = dynamic_cast<RigidBody*>(pair2.second->get_Components().at("RigidBody"));
									if (currentObject->get_Name() == "Robot") // Player Shooting Enemy
									{
										if (pair2.second->get_Tag() == "Enemy")
										{
											colChecks.insert(std::make_pair(bullet, pair2.second));
											dynamic_cast<Character*>(pair2.second->get_Components().at("Character"))->loseLife();
											if (dynamic_cast<Character*>(pair2.second->get_Components().at("Character"))->getHealth() == 0)
											{
												pair2.second->set_ToDelete();
											}
										}			
										else if (pair2.second->get_Tag() == "Light")
										{

										}
										else if (pair2.second->get_Tag() == "Player")
										{

										}
										else if (!tempBody->get_Moveable())
										{
											//Stop the object moving in the current direction
											colChecks.insert(std::make_pair(bullet, pair2.second));
										}
									}
									else // Enemy Shooting Player
									{
										if (pair2.second->get_Tag() == "Player")
										{
											colChecks.insert(std::make_pair(bullet, pair2.second));
											dynamic_cast<Character*>(pair2.second->get_Components().at("Character"))->loseLife();
											if (dynamic_cast<Character*>(pair2.second->get_Components().at("Character"))->getHealth() == 0)
											{
												pair2.second->set_ToDelete();
											}
										}
										else if (pair2.second->get_Tag() == "Enemy")
										{

										}
										else
										{
											colChecks.insert(std::make_pair(bullet, pair2.second));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	

	if (colChecks.size() != 0)
	{
		for (auto const& pair : colChecks)
		{
			dynamic_cast<BoxCollider_3D*>(pair.first->get_Components().at("BoxCollider_3D"))->setCollisionCheck(true);
		}
	}
	else
	{
		if (gameObjects.count("Robot"))
		{
			dynamic_cast<BoxCollider_3D*>(gameObjects.at("Robot")->get_Components().at("BoxCollider_3D"))->setCollisionCheck(false);
		}
		
		for (auto const& pair : gameObjects)
		{

			if (pair.second->get_Tag() == "Particle") { continue; }
			if (static_cast<GameObject_3D*>(pair.second)->get_BulletList().size() != 0)
			{
				for (unsigned int i = 0; i < static_cast<GameObject_3D*>(pair.second)->get_BulletList().size(); i++)
				{
					static_cast<BoxCollider_3D*>(static_cast<GameObject_3D*>(pair.second)->get_BulletList()[i]->get_Components().at("BoxCollider_3D"))->setCollisionCheck(false);
				}
			}
		}
	}
}