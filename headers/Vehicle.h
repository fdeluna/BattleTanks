#ifndef Vehicle_H
#define Vehicle_H

#include <Ogre.h>
#include "Enums.h"

class Vehicle
{
public:
	Vehicle(int heigh, int width, VehicleType type, Ogre::Entity *ent, Ogre::SceneNode *node);
	Vehicle();
	~Vehicle();

	const bool isDead();
	const int getHeigh();
	const int getWidth();
	const int getHP();
	int getType();
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
	int _heigh;
	int _width;
	int _hp;
	int _type;	
	Ogre::SceneNode *_node = NULL;
	Ogre::Entity *_entity = NULL;
	//lista nodos
};

#endif