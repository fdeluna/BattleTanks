#include "IntroState.h"
#include "GridNode.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter()
{
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "IntroState");
	_camera = _sceneMgr->createCamera("IntroCamera");
	_camera->setPosition(0, -0, -10);
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);

	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	//_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));


	_exitGame = false;
}

void IntroState::exit()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause()
{
}

void IntroState::resume()
{
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt)
{	
	Entity *gridTile = _sceneMgr->createEntity( "Cube1.mesh");	
	OgreGrid* tile = static_cast<GridNode *> (_sceneMgr->createSceneNode("Test"));
	tile->getScale();
	tile->attachObject(gridTile);
	tile->setPosition(0, 0, 0);
			
	_sceneMgr->getRootSceneNode()->addChild(tile);
	
	return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void IntroState::keyPressed(const OIS::KeyEvent &e)
{

}

void IntroState::keyReleased (const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE)
	{
		_exitGame = true;
	}
}

void IntroState::mouseMoved (const OIS::MouseEvent &e)
{
}

void IntroState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void IntroState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState* IntroState::getSingletonPtr()
{
	return msSingleton;
}

IntroState& IntroState::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}