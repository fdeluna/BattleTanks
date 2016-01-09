#ifndef Vehicle_H
#define Vehicle_H

#include <Ogre.h>
#include "Enums.h"

class Vehicle
{
public:
	Vehicle(int heigh, int width, VehicleType type, Ogre::String name, Ogre::SceneManager &sceneMgr);
	Vehicle();
	~Vehicle();

	const bool isDead();
	const int getHeigh();
	const int getWidth();
	const int getHP();
	VehicleType getType();
	Ogre::Entity* getEntity();
	Ogre::SceneNode* getNode();

	void setEntity(Ogre::Entity &ent);
	void setNode(Ogre::SceneNode &ent);
	void setPosition(const Ogre::Vector3 &pos, Ogre::Real &yaw);
	void setHeigh(int heigh);
	void setWidth(int width);
	void setHP(int hp);

	void decreaseHP();
	void render();
	void addGridNode(Ogre::SceneNode* gridnode);

private:
	bool _renderer = false;
	bool dead = false;
	int _heigh;
	int _width;
	int _hp;
	VehicleType _type;
	Ogre::SceneNode *_node = NULL;
	Ogre::Entity *_entity = NULL;

	std::vector<Ogre::SceneNode*> _gridNodes;
	
};

#endif