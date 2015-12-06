#include "PlayState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter() {
	std::vector<std::vector<bool> > _matrixPositionBusy(10,
		std::vector<bool>(10));

	MovableObject::setDefaultQueryFlags(0);
	_root = Ogre::Root::getSingletonPtr();
	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "PlayState");
	// set camera
	_camera = _sceneMgr->createCamera("IntroCamera");
	_camera->setPosition(0, 45, -20);//0,40,-20
	_camera->lookAt(Ogre::Vector3(0, 0, 0));//0,0,0
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);

	// add viewport
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

	_exitGame = false;

	// set our skybox
	_sceneMgr->setSkyBox(true, "SkyBox", 5000);
	/*
	 // Player gird//Show name of clicked sceneNode
	 std::stringstream nodoAleatorioNombre;
	 nodoAleatorioNombre.str("");
	 nodoAleatorioNombre << _matrixPositionBusy[1][1];
	 formatWin->getChild("Text1")->setText(nodoAleatorioNombre.str());
	 Ogre::SceneNode* node1 = _sceneMgr->createSceneNode("PlayerGrid");
	 node1->setPosition(2, 0, -5);
	 node1->setScale(3.5, 0.5, 3.5);
	 createGrid(node1,NULL,_playerGrid);
	 */
	// IA gird
	Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("IAGrid");
	node2->setPosition(0, 0, 10);
	//node2->setScale(3.5, 0.5, 3.5);
	createGrid(node2, GRIDNODE, _enemyGrid);

	// Add grids to scene
	//_sceneMgr->getRootSceneNode()->addChild(node1);
	_sceneMgr->getRootSceneNode()->addChild(node2);
	_selectedNode = NULL;

	createGUI();
	Ogre::SceneNode* nodeTank = _sceneMgr->createSceneNode("Tank");
	Ogre::Entity* entityTank = _sceneMgr->createEntity("Tank",
		"Cube.057.mesh");
	Ogre::SceneNode* nodeTank2 = _sceneMgr->createSceneNode("Tank2");
	Ogre::Entity* entityTank2 = _sceneMgr->createEntity("Tank2",
		"Cube.057.mesh");
	Ogre::SceneNode* nodeTank3 = _sceneMgr->createSceneNode("Tank3");
	Ogre::Entity* entityTank3 = _sceneMgr->createEntity("Tank3",
		"Cube.057.mesh");
	Ogre::SceneNode* nodeTank4 = _sceneMgr->createSceneNode("Tank4");
	Ogre::Entity* entityTank4 = _sceneMgr->createEntity("Tank4",
		"Cube.057.mesh");
	Ogre::SceneNode* nodeTank5 = _sceneMgr->createSceneNode("Tank5");
	Ogre::Entity* entityTank5 = _sceneMgr->createEntity("Tank5",
		"Cube.057.mesh");

	node2->addChild(nodeTank);
	node2->addChild(nodeTank2);
	node2->addChild(nodeTank3);
	node2->addChild(nodeTank4);
	node2->addChild(nodeTank5);

	Vehicle* tank = new Vehicle(2, 2, VehicleType::TANK, entityTank, nodeTank);
	Vehicle* tank2 = new Vehicle(2, 2, VehicleType::TANK, entityTank2, nodeTank2);
	Vehicle* tank3 = new Vehicle(2, 2, VehicleType::TANK, entityTank3, nodeTank3);
	Vehicle* tank4 = new Vehicle(2, 2, VehicleType::TANK, entityTank4, nodeTank4);
	Vehicle* tank5 = new Vehicle(2, 2, VehicleType::TANK, entityTank5, nodeTank5);

	std::cout << putVehicle(_enemyGrid, *tank, 0, 0);

	srand(time(NULL));
	int randI = 1;
	int randJ = 0;

	while (!putVehicle(_enemyGrid, *tank2, randI, randJ))
	{
		randI = rand() % 9;
		randJ = rand() % 9;
	}

	while (!putVehicle(_enemyGrid, *tank3, randI, randJ))
	{
		randI = rand() % 9;
		randJ = rand() % 9;
	}


	while (!putVehicle(_enemyGrid, *tank4, randI, randJ))
	{
		randI = rand() % 9;
		randJ = rand() % 9;
	}

	while (!putVehicle(_enemyGrid, *tank5, randI, randJ))
	{
		randI = rand() % 9;
		randJ = rand() % 9;
	}
	//std::cout << putVehicle(_enemyGrid, *tank2, 0, 0);

	//putTank(node2, _matrixPositionBusy);
	//putTruck(node2, _matrixPositionBusy);
	//putMissileLauncher(node2, _matrixPositionBusy);
	//putSoldiers(node2, _matrixPositionBusy);


}

void PlayState::exit() {
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause() {
}

void PlayState::resume() {
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
		evt.timeSinceLastFrame);

	// switch ESTADO PLAY
	// INICIO- PONEMOS LOS BARCOS
	// JUEGO - CONTROLAMOS TURNOS
	// bool IA = false;
	// FIN - MOSTRAMOS PUNTACION + GAMEOVER

	return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e) {
}

void PlayState::keyReleased(const OIS::KeyEvent &e) {
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void PlayState::mouseMoved(const OIS::MouseEvent &e) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
		e.state.X.rel, e.state.Y.rel);

	Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
	Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;

	std::stringstream nombre;
	if (_selectedNode) {
		nombre.str("");
		nombre << _selectedNode->getName();
		formatWin->getChild("Text2")->setText(nombre.str());
	}

	//OPTIMIZE // REFACTOR
	//MOUSE OVER BEHAVIOUR
	SceneNode* aux = getSceneNode(normX, normY, GRIDNODE);

	if (!_selectedNode && aux) {
		_selectedNode = aux;
	}

	if (_selectedNode != aux) {
		static_cast<Entity*> (_selectedNode->getAttachedObject(0))->setMaterialName(
			"Cube1");
		_selectedNode = aux;
	}
	else if (_selectedNode) {
		static_cast<Entity*> (_selectedNode->getAttachedObject(0))->setMaterialName(
			"red");
	}

}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
		convertMouseButton(id));
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
		convertMouseButton(id));

	if (convertMouseButton(id) == CEGUI::LeftButton) {
		std::stringstream posicion2;
		posicion2 << "(" << e.state.X.abs << "," << e.state.Y.abs << ","
			<< e.state.Z.abs << ")";
		formatWin->getChild("Text3")->setText(posicion2.str());
		Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
		Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;
		SceneNode* aux = getSceneNode(normX, normY, GRIDNODE);

		// TODO USE PRIVATE NODE TO GET SELECTED_NODE
		if (aux){

			static_cast<Entity*>(aux->getAttachedObject(0))->setQueryFlags(NULL);
			static_cast<Entity*> (aux->getAttachedObject(0))->setMaterialName(
				"Cube1");

			_selectedNode = NULL;
			Vehicle* vAux = NULL;
			try
			{
				vAux = Ogre::any_cast<Vehicle*>(aux->getUserObjectBindings().getUserAny());
			}
			catch (Ogre::Exception& e){}

			if (vAux)
			{
				vAux->decreaseHP();
				if (vAux->isDead()){
					vAux->render();
				}
				Ogre::ParticleSystem* partSystem = _sceneMgr->createParticleSystem(aux->getName() + "Smoke", "Smoke");
				aux->attachObject(partSystem);
			}			
			// PARICULE SIZE WHEN MODEL IS DISPLAYED
			//partSystem->setParameter("particle_width","3");
			//partSystem->setParameter("particle_height", "3");						
		}
	}
}

PlayState* PlayState::getSingletonPtr() {
	return msSingleton;
}

PlayState& PlayState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

void PlayState::createGUI() {

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

	//Ajuste de tama�o y posicion del layout
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
	//CEGUI::Event::Subscriber(&PlayState::exit, this));

	//Attaching buttons
	sheet->addChild(formatWin);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	// INITIALISE OIS MOUSE POSITION TO CEGUI MOUSE POSITION
	OIS::MouseState
		&mutableMouseState =
		const_cast<OIS::MouseState &> (GameManager::getSingletonPtr()->getInputManager()->getMouse()->getMouseState());
	mutableMouseState.X.abs
		= CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x;
	mutableMouseState.Y.abs
		= CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y;
}

//Conversion mouse
CEGUI::MouseButton PlayState::convertMouseButton(OIS::MouseButtonID id) {
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
SceneNode* PlayState::getSceneNode(Ogre::Real const &x, Ogre::Real const &y,
	uint32 mask) {

	std::stringstream posicion;
	posicion << "(" << x << "," << y << ")";
	formatWin->getChild("Text4")->setText(posicion.str());
	// Ray to mouse position	
	RaySceneQuery* _raySceneQuery = _sceneMgr->createRayQuery(
		_camera->getCameraToViewportRay(x, y));
	_raySceneQuery->setSortByDistance(true);
	_raySceneQuery->setQueryMask(mask);

	Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator it;
	it = result.begin();

	if (it != result.end()) {
		return it->movable->getParentSceneNode();
	}
	return NULL;
}
// Creategrid from node position
void PlayState::createGrid(SceneNode* gridNode, uint32 mask,
	std::vector<std::vector<Ogre::SceneNode*>> &_grid) {

	std::vector<SceneNode*> nodeRow;
	std::stringstream nameNode;
	std::stringstream nameEntity;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			// Build unique name for child node
			nameNode.str("");
			nameEntity.str("");
			nameNode << gridNode->getName() << "node_" << i << '_' << j;
			nameEntity << gridNode->getName() << "entity_" << i << '_' << j;
			Ogre::Entity* ent1 = _sceneMgr->createEntity(nameEntity.str(),
				"Cube1.mesh");
			// Add mask to select node
			ent1->setQueryFlags(mask);
			Ogre::SceneNode* node = _sceneMgr->createSceneNode(nameNode.str());
			node->attachObject(ent1);
			node->setPosition(Ogre::Vector3(j * -2, 0, i * -2));
			node->setScale(3.5, 3.5, 3.5);
			node->getUserObjectBindings().setUserAny(Ogre::Any(NULL));
			// Attach child nodes to gridNode			
			nodeRow.push_back(node);
			gridNode->addChild(node);
		}
		_grid.push_back(nodeRow);
		nodeRow.clear();
	}
}

// TODO COMMENT
bool PlayState::putVehicle(std::vector< std::vector< Ogre::SceneNode*>> &gridNode, Vehicle &vehicle, int x, int z)
{
	bool allow = true;
	int randOrientacion = rand() % 2;

	int heigh = randOrientacion == 1 ? vehicle.getHeigh() : vehicle.getWidth();
	int width = randOrientacion == 1 ? vehicle.getWidth() : vehicle.getHeigh();

	std::vector<SceneNode*> vNodes;

	for (int i = x; i < heigh + x; i++)
	{
		for (int j = z; j < width + z && allow; j++)
		{
			Vehicle* vAux = NULL;
			try
			{
				vAux = Ogre::any_cast<Vehicle*>(gridNode[i][j]->getUserObjectBindings().getUserAny());
			}
			catch (Ogre::Exception& e){}

			if (vAux)
			{
				allow = false;
				break;
			}
			else
			{
				gridNode[i][j]->getUserObjectBindings().setUserAny(Ogre::Any(&vehicle));
				vNodes.push_back(gridNode[i][j]);
			}			
		}
	}

	if (!allow)
	{
		for (int i = 0; i < vNodes.size(); i++)
		{
			vNodes[i]->getUserObjectBindings().setUserAny(Ogre::Any(NULL));
		}
	}
	else
	{
		std::stringstream nodeName;
		nodeName << "Vehicle_" << x << "_" << z;
		vehicle.getNode()->setPosition(gridNode[x][z]->getPosition());
		vehicle.getNode()->translate(-1, 2.3, -1);
		vehicle.getNode()->yaw(Degree(90));
		vehicle.getNode()->setScale(0.28, 0.28, 0.2);

		//vehicle.render();
		std::cout << nodeName.str() << std::endl;
		std::cout << "X:" << x << std::endl;
		std::cout << "Z:" << z << std::endl;
		//Change to gridNode
		//gridNode[x][z]->addChild(nodeTank);
		//_sceneMgr->getRootSceneNode()->addChild(vehicle.getNode());
	}
	return allow;
}