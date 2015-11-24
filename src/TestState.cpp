#include "TestState.h"
#include "GridNode.h"

template<> TestState* Ogre::Singleton<TestState>::msSingleton = 0;

void TestState::enter()
{
	MovableObject::setDefaultQueryFlags(0);
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "TestState");
	_camera = _sceneMgr->createCamera("IntroCamera");
	_camera->setPosition(0, 30, -22);
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);

	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	//_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

	Entity *gridTile = _sceneMgr->createEntity("Test", "Cube1.mesh");
	_exitGame = false;

	_sceneMgr->setSkyBox(true, "SkyBox", 5000);  // set our skybox

	_raySceneQuery = _sceneMgr->createRayQuery(Ray());

	// GRID TEST
	Ogre::SceneNode* node1 = _sceneMgr->createSceneNode("PlayerGrid");
	node1->setPosition(2, 0, -5);
	node1->setScale(3.5, 0, 3.5);
	std::stringstream nameNode;
	std::stringstream nameEntity;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			nameNode << "node_" << i << '_' << j;
			nameEntity << "entity_" << i << '_' << j;
			Ogre::Entity* ent1 = _sceneMgr->createEntity(nameEntity.str(), "Cube1.mesh");
			ent1->setQueryFlags(GRIDNODE);
			Ogre::SceneNode* node = _sceneMgr->createSceneNode(nameNode.str());
			node->attachObject(ent1);
			node->setPosition(Ogre::Vector3(i*0.5, 0, j*0.5));
			node1->addChild(node);
		}
	}

	Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("IAGrid");
	node2->setPosition(-17, 0, -5);
	node2->setScale(3.5, 0, 3.5);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			nameNode << "node__" << i << '_' << j;
			nameEntity << "entity__" << i << '_' << j;
			Ogre::Entity* ent1 = _sceneMgr->createEntity(nameEntity.str(), "Cube1.mesh");			
			Ogre::SceneNode* node = _sceneMgr->createSceneNode(nameNode.str());
			node->attachObject(ent1);
			node->setPosition(Ogre::Vector3(i*0.5, 0, j*0.5));
			node2->addChild(node);
		}
	}

	_sceneMgr->getRootSceneNode()->addChild(node1);
	_sceneMgr->getRootSceneNode()->addChild(node2);
	_selectedNode = NULL;

}

void TestState::exit()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void TestState::pause()
{
}

void TestState::resume()
{
}

bool TestState::frameStarted(const Ogre::FrameEvent& evt)
{

	return true;
}

bool TestState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void TestState::keyPressed(const OIS::KeyEvent &e)
{

}

void TestState::keyReleased(const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE)
	{
		_exitGame = true;
	}
}

void TestState::mouseMoved(const OIS::MouseEvent &e)
{
	int x = e.state.X.abs;
	int y = e.state.Y.abs;
	//	std::cout << "MOVED" << std::endl;
	_mouseRay = _camera->getCameraToViewportRay(x / float(_root->getAutoCreatedWindow()->getWidth()), y / float(_root->getAutoCreatedWindow()->getHeight()));
	//Ogre::Ray rayMouse = _camera->getCameraToViewportRay(posx / float(_root->getAutoCreatedWindow()->getWidth()), posy / float(_root->getAutoCreatedWindow()->getHeight()));
	_raySceneQuery->setRay(_mouseRay);
	_raySceneQuery->setSortByDistance(true);
	_raySceneQuery->setQueryMask(GRIDNODE);

	Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator it;
	it = result.begin();

	if (_selectedNode)
	{
		_selectedNode->showBoundingBox(false);
		static_cast<Entity*>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube1");
	}

	if (it != result.end()){
		_selectedNode = it->movable->getParentSceneNode();
		Entity* ent = static_cast<Entity*>(it->movable->getParentSceneNode()->getAttachedObject(0));
		std::cout << static_cast<Entity*>(it->movable->getParentSceneNode()->getAttachedObject(0))->getQueryFlags();
		_selectedNode->showBoundingBox(true);
		static_cast<Entity*>(it->movable->getParentSceneNode()->getAttachedObject(0))->setMaterialName("red");
	}
}

void TestState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void TestState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

TestState* TestState::getSingletonPtr()
{
	return msSingleton;
}

TestState& TestState::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}