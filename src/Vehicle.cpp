#include "Vehicle.h"

Vehicle::Vehicle(int heigh, int width, VehicleType type, Ogre::String name, Ogre::SceneManager &sceneMgr)
{
	_heigh = heigh;
	_width = width;
	_type = type;
	_node = sceneMgr.createSceneNode(name);
	switch (type)
	{
	case VehicleType::SOLDIER:
		_hp = 2;

		_entity = sceneMgr.createEntity(name,
			"Cube.057.mesh");
		break;
	case VehicleType::TRUCK:
		_hp = 3;
		_entity = sceneMgr.createEntity(name,
			"Cube.057.mesh");
		break;
	case VehicleType::TANK:
		_hp = 4;
		_entity = sceneMgr.createEntity(name,
			"Cube.057.mesh");
		break;
	case VehicleType::MISSILE_LAUNCHER:
		_hp = 6;
		_entity = sceneMgr.createEntity(name,
			"Cube.057.mesh");
		break;
	}
}


Vehicle::Vehicle()
{
	_heigh = 0;
	_width = 0;
	_hp = 0;
	_node = NULL;
	_entity = NULL;
}

Vehicle::~Vehicle()
{
	_node = NULL;
	_entity = NULL;
}

const bool Vehicle::isDead()
{
	return _hp <= 0 ? true : false;
}

const int Vehicle::getHeigh()
{
	return _heigh;
}

const int Vehicle::getWidth()
{
	return _width;
}

const int Vehicle::getHP()
{
	return _hp;
}

VehicleType Vehicle::getType()
{
	return _type;
}

Ogre::SceneNode* Vehicle::getNode()
{
	return _node;
}

Ogre::Entity* Vehicle::getEntity()
{
	return _entity;
}

void Vehicle::setNode(Ogre::SceneNode &node)
{
	_node = &node;
}

void Vehicle::setEntity(Ogre::Entity &ent)
{
	_entity = &ent;
}

void Vehicle::decreaseHP()
{
	if (_hp > 0)
		_hp--;
}

void Vehicle::render()
{
	if (!_renderer)
	{
		_node->attachObject(_entity);
		_renderer = true;
	}
}