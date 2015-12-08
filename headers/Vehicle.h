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
	void setHeigh(int heigh);
	void setWidth(int width);
	void setHP(int hp);

	void decreaseHP();
	void render();

private:
	bool _renderer = false;
	int _heigh;
	int _width;
	int _hp;
	VehicleType _type;
	Ogre::SceneNode *_node = NULL;
	Ogre::Entity *_entity = NULL;
	//lista nodos
};

#endif