#include "TestState.h"
#include "GridNode.h"

template<> TestState* Ogre::Singleton<TestState>::msSingleton = 0;

void TestState::enter()
{
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "TestState");
	_camera = _sceneMgr->createCamera("IntroCamera");
	_camera->setPosition(0, -0, -10);
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);

	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	//_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

	Entity *gridTile = _sceneMgr->createEntity("Test","Cube1.mesh");
	_exitGame = false;
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
	Entity *gridTile = _sceneMgr->createEntity( "Cube1.mesh");	
	GridNode* tile = static_cast<GridNode *> (_sceneMgr->createSceneNode());
	tile->getScale();
	tile->attachObject(gridTile);
	tile->setPosition(0, 0, 0);
			
	_sceneMgr->getRootSceneNode()->addChild(tile);
	
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

void TestState::keyReleased (const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE)
	{
		_exitGame = true;
	}
}

void TestState::mouseMoved (const OIS::MouseEvent &e)
{
}

void TestState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void TestState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
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