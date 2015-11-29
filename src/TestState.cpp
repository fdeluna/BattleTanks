#include "TestState.h"


template<> TestState* Ogre::Singleton<TestState>::msSingleton = 0;

void TestState::enter()
{
	MovableObject::setDefaultQueryFlags(0);
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "TestState");
	// set camera
	_camera = _sceneMgr->createCamera("IntroCamera");
	_camera->setPosition(0, 40, -20);
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);

	// add viewport
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);	
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
	
	_exitGame = false;

	// set our skybox
	_sceneMgr->setSkyBox(true, "SkyBox", 5000);  

	// Player gird
	Ogre::SceneNode* node1 = _sceneMgr->createSceneNode("PlayerGrid");
	node1->setPosition(2, 0, -5);
	node1->setScale(3.5, 0.5, 3.5);
	createGrid(node1,NULL,_enemyGrid);

	// IA gird
	Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("IAGrid");
	node2->setPosition(-17, 0, -5);
	node2->setScale(3.5, 0.5, 3.5);
	createGrid(node2, GRIDNODE, _enemyGrid);

	// Add grids to scene
	_sceneMgr->getRootSceneNode()->addChild(node1);
	_sceneMgr->getRootSceneNode()->addChild(node2);
	_selectedNode = NULL;

	createGUI();
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
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(evt.timeSinceLastFrame);

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
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);

	int x = e.state.X.abs;
	int y = e.state.Y.abs;

	Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
	Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;

	//Mostrar en el Text1 del layout la posicion del puntero
	std::stringstream posicion;
	posicion << "(" << e.state.X.rel << "," << e.state.Y.rel << ","
		<< e.state.Z.rel << ")";
	formatWin->getChild("Text1")->setText(posicion.str());	

	//OPTIMIZE // REFACTOR
	//MOUSE OVER BEHAVIOUR
	SceneNode* aux = getSceneNode(normX, normY, GRIDNODE);
	
	if (!_selectedNode && aux){
		_selectedNode = aux;
	}


	if (_selectedNode != aux)
	{
		static_cast<Entity*>(_selectedNode->getAttachedObject(0))->setMaterialName("Cube1");
		_selectedNode = aux;
	}
	else if (_selectedNode)
	{
		static_cast<Entity*>(_selectedNode->getAttachedObject(0))->setMaterialName("red");
	}


}

void TestState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
		convertMouseButton(id));
}

void TestState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
		convertMouseButton(id));
	if (convertMouseButton(id) == CEGUI::LeftButton)
	{
		std::stringstream posicion2;
		posicion2 << "(" << e.state.X.abs << "," << e.state.Y.abs << ","
			<< e.state.Z.abs << ")";
		formatWin->getChild("Text3")->setText(posicion2.str());
		// TODO USE PRIVATE NODE TO GET SELECTED_NODE
		if (_selectedNode){					
			Ogre::ParticleSystem* partSystem = _sceneMgr->createParticleSystem(_selectedNode->getName()+"Smoke", "Smoke");						
			// PARICULE SIZE WHEN MODEL IS DISPLAYED
			//partSystem->setParameter("particle_width","3");
			//partSystem->setParameter("particle_height", "3");
			static_cast<Entity*>(_selectedNode->getAttachedObject(0))->setQueryFlags(NULL);
			_selectedNode->attachObject(partSystem);
			//_selectedNode->getParentSceneNode()->removeChild(_selectedNode);										
		}
	}

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

void TestState::createGUI() {

	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(
		"DejaVuSans-12");

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
		"TaharezLook/MouseArrow");

	// load all the fonts (if they are not loaded yet)
	CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
		"DefaultWindow", "Sheet");

	//Config Window
	formatWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
		"stringFormat.layout");

	//Ajuste de tamaño y posicion del layout
	formatWin->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.38, 0)));
	formatWin->setPosition(
		CEGUI::UVector2(CEGUI::UDim(0.03, 0), CEGUI::UDim(0.6, 0)));

	//Setting Text!
	//Si quieres que la primera vez que se ejecute GUI los textBox muestren algo por defecto
	formatWin->getChild("Text1")->setText("Click para posicion");
	//formatWin->getChild("Text2")->setText();
	//formatWin->getChild("Text3")->setText();
	//formatWin->getChild("Text4")->setText();
	//formatWin->getChild("Text5")->setText();

	//Exit Window
	//Asignar quit() al boton Exit
	//CEGUI::Window* exitButton = formatWin->getChild("ExitButton");
	//xitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
	//CEGUI::Event::Subscriber(&TestState::exit, this));

	//Attaching buttons
	sheet->addChild(formatWin);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);



	// INITIALISE OIS MOUSE POSITION TO CEGUI MOUSE POSITION
	OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(GameManager::getSingletonPtr()->getInputManager()->getMouse()->getMouseState());
	mutableMouseState.X.abs = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x;
	mutableMouseState.Y.abs = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y;
}

//Conversion mouse
CEGUI::MouseButton TestState::convertMouseButton(OIS::MouseButtonID id) {
	CEGUI::MouseButton ceguiId;
	switch (id) {
	case OIS::MB_Left:
		ceguiId = CEGUI::LeftButton;
		break;
	case OIS::MB_Right:
		ceguiId = CEGUI::RightButton;
		break;
	case OIS::MB_Middle:
		ceguiId = CEGUI::MiddleButton;
		break;
	default:
		ceguiId = CEGUI::LeftButton;
	}
	return ceguiId;
}

// Return node from ray match with mask
SceneNode* TestState::getSceneNode(Ogre::Real const &x, Ogre::Real const &y, uint32 mask)
{

	std::stringstream posicion;
	posicion << "(" << x << "," << y << ")";
	formatWin->getChild("Text4")->setText(posicion.str());
	// Ray to mouse position	
	RaySceneQuery* _raySceneQuery = _sceneMgr->createRayQuery(_camera->getCameraToViewportRay(x, y));
	_raySceneQuery->setSortByDistance(true);
	_raySceneQuery->setQueryMask(mask);

	Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator it;
	it = result.begin();

	if (it != result.end()){
		return it->movable->getParentSceneNode();
	}
	return NULL;
}
// Creategrid from node position
void TestState::createGrid(SceneNode*  gridNode, uint32 mask, std::vector<SceneNode*>  &_grid)
{		
	std::stringstream nameNode;
	std::stringstream nameEntity;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			// Build unique name for child node
			nameNode << gridNode->getName() << "_node_" << i << '_' << j;
			nameEntity << gridNode->getName() << "entity_" << i << '_' << j;
			Ogre::Entity* ent1 = _sceneMgr->createEntity(nameEntity.str(), "Cube1.mesh");
			// Add mask to select node
			ent1->setQueryFlags(mask);
			Ogre::SceneNode* node = _sceneMgr->createSceneNode(nameNode.str());
			node->attachObject(ent1);
			node->setPosition(Ogre::Vector3(i*0.5, 0, j*0.5));
			// Attach child nodes to gridNode
			gridNode->addChild(node);
		}
	}
}