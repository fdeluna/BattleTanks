#include "Vehicle.h"

Vehicle::Vehicle(int heigh, int width, VehicleType type, Ogre::String name, Ogre::SceneManager &sceneMgr)
{
	_heigh = heigh;
	_width = width;
	_type = type;
	_node = sceneMgr.createSceneNode(name);
	_entity = sceneMgr.createEntity(name,
		"Cube.057.mesh");
	_node->attachObject(_entity);
	_node->setVisible(false);
	switch (type)
	{
	case VehicleType::SOLDIER:
		_hp = 2;
		_node->setScale(0.15, 0.3, 0.15);	
		break;
	case VehicleType::TRUCK:
		_hp = 3;
		_node->setScale(0.15, 0.3, 0.4);
		break;
	case VehicleType::TANK:
		_hp = 4;
		_node->setScale(0.29, 0.3, 0.29);
		break;
	case VehicleType::MISSILE_LAUNCHER:
		_hp = 6;
		_node->setScale(0.29, 0.3, 0.4);
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
	bool isDead = _hp <= 0 ? true : false;
	if (isDead && !dead)
	{
		Ogre::ParticleSystem* smoke = NULL;
		for (Ogre::SceneNode* node : _gridNodes)
		{
			std::cout << node->getAttachedObject(Ogre::String(node->getName()).append("_Smoke"))->getName();
			smoke = static_cast<Ogre::ParticleSystem*>(node->getAttachedObject(Ogre::String(node->getName()).append("_Smoke")));
			node->detachObject(Ogre::String(node->getName()).append("_Smoke"));
		}			
		smoke->getEmitter(0)->setPosition(Ogre::Vector3(0, 10, 0));
		_node->attachObject(smoke);
		dead = true;
	}
	return isDead;
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

void Vehicle::setPosition(const Ogre::Vector3 &pos, Ogre::Real &yaw)
{
	_node->setPosition(pos);
	_node->yaw(Ogre::Degree(yaw));
	switch (_type)
	{
	case VehicleType::SOLDIER:
		if (yaw == 90)
			_node->translate(-1, 0, 0);
		else
			_node->translate(0, 0, -1);
		break;
	case VehicleType::TRUCK:
		if (yaw == 90)
			_node->translate(-2, 0, 0);
		else
			_node->translate(0, 0, -1);
		break;
	case VehicleType::TANK:
		_node->translate(-1, 0, -1);
		break;
	case VehicleType::MISSILE_LAUNCHER:
		if (yaw == 90)
			_node->translate(-2, 0, -1);
		else
			_node->translate(-1, 0, -2);
		break;
	}
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
		_node->setVisible(true);
		_renderer = true;
	}
}

void Vehicle::addGridNode(Ogre::SceneNode* gridnode)
{
	_gridNodes.push_back(gridnode);
}