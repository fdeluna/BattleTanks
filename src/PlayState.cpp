#include "PlayState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter() {

	MovableObject::setDefaultQueryFlags(0);

	// Initialize Game flow
	_gameflow = GameFlow::INIT;


	// create scene manager for state
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
	srand(time(NULL));
	createGUI();
}

void PlayState::exit() 
{	
	
	_playerGrid.clear();
	_IAGrid.clear();
	_playerVehicles.clear();
	_IAVehicles.clear();
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

	switch (_gameflow)
	{
	case GameFlow::INIT:
	{
		// Player grid
		Ogre::SceneNode* node1 = _sceneMgr->createSceneNode("PlayerGrid");
		node1->setPosition(20, 0, 10);
		createGrid(node1, PLAYER_GRIDNODE, _playerGrid, _playerVehicles);
		
		// Player vehicles
		for (Vehicle *v : _playerVehicles)
		{
			int randX = rand() % 9;
			int randZ = rand() % 9;
			while (!putVehicle(_playerGrid, *v, randX, randZ, true))
			{
				randX = rand() % 9;
				randZ = rand() % 9;
			}
		}

		// IA grid
		Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("IAGrid");
		node2->setPosition(-5, 0, 10);
		createGrid(node2, IA_GRID, _IAGrid, _IAVehicles);

		// IA vehicles
		for (Vehicle *v : _IAVehicles)
		{
			int randX = rand() % 9;
			int randZ = rand() % 9;
			while (!putVehicle(_IAGrid, *v, randX, randZ, false))
			{
				randX = rand() % 9;
				randZ = rand() % 9;
			}
		}

		// Add grids to scene
		_sceneMgr->getRootSceneNode()->addChild(node1);
		_sceneMgr->getRootSceneNode()->addChild(node2);
		_selectedNode = NULL;
		
		_gameflow = GameFlow::PLAY;
		_iaTurn = rand() % 2;
	}
	break;
	case GameFlow::PLAY:
	{
		if (_iaTurn)
		{		
			_contador += evt.timeSinceLastFrame;
			std::cout << "****************" << std::endl;
			std::cout << "TIEMPO:" << _contador;
			std::cout << "****************" << std::endl;
			if (_contador >= 2.5){
				try{
					int randX = rand() % 10;
					int randZ = rand() % 10;
					if (_playerGrid[randX][randZ]->getAttachedObject(0)->getQueryFlags())
					{
						while (!fire(_playerGrid[randX][randZ]))
						{
							randX = rand() % 10;
							randZ = rand() % 10;
						}
						std::cout << "****************" << std::endl;
						std::cout << "DISPARADO: X=" << randX << "," << "Z=" << randZ << std::endl;
						std::cout << "****************" << std::endl;
						//_contador++;						
						_iaTurn = false;
						_contador = rand() % 2 + 1;									

					}
					else
					{
						_iaTurn = true;
					}
				}
				catch (Ogre::Exception& e)
				{
					_iaTurn = true;
				}
			}
		}
	}
	break;
	case GameFlow::GAMEOVER:
	{		
		changeState(PlayState::getSingletonPtr());
	}
	break;
	}


	// switch ESTADO PLAY
	// INICIO- PONEMOS LOS BARCOS
	// JUEGO - CONTROLAMOS TURNOS
	// bool IA = false;
	// FIN - MOSTRAMOS PUNTACION + GAMEOVER

	return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt) 
{
	if (lose(_playerVehicles))
	{
		std::cout << "IA WINS";
		_gameflow = GameFlow::GAMEOVER;
	}
	if (lose(_IAVehicles))
	{
		std::cout << "Players WINS";
		_gameflow = GameFlow::GAMEOVER;
	}

	if (_exitGame)
		return false;

	return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e) 
{
}

void PlayState::keyReleased(const OIS::KeyEvent &e) 
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void PlayState::mouseMoved(const OIS::MouseEvent &e) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
		e.state.X.rel, e.state.Y.rel);

	if (!_iaTurn)
	{
		Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
		Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;

		std::stringstream nombre;
		if (_selectedNode) {
			nombre.str("");
			nombre << _selectedNode->getName();
			formatWin->getChild("Text2")->setText(nombre.str());
		}

		SceneNode* aux = getSceneNode(normX, normY, IA_GRID);

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
}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
		convertMouseButton(id));
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
		convertMouseButton(id));

	if (!_iaTurn)
	{
		if (convertMouseButton(id) == CEGUI::LeftButton) {
			std::stringstream posicion2;
			posicion2 << "(" << e.state.X.abs << "," << e.state.Y.abs << ","
				<< e.state.Z.abs << ")";
			formatWin->getChild("Text3")->setText(posicion2.str());
			Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
			Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;
			SceneNode* aux = getSceneNode(normX, normY, IA_GRID);
			_iaTurn = fire(aux);
		}
	}
}

bool PlayState::fire(SceneNode* node)
{
	bool fire = false;
	_selectedNode = NULL;
	if (node)
	{
		static_cast<Entity*>(node->getAttachedObject(0))->setQueryFlags(NULL);
		static_cast<Entity*> (node->getAttachedObject(0))->setMaterialName("Crater");
		Vehicle* vAux = NULL;
		try
		{
			vAux = Ogre::any_cast<Vehicle*>(node->getUserObjectBindings().getUserAny());
		}
		catch (Ogre::Exception& e){}

		if (vAux)
		{
			vAux->decreaseHP();
			if (vAux->isDead()){
				vAux->render();
			}
			Ogre::ParticleSystem* partSystem = _sceneMgr->createParticleSystem(node->getName() + "Smoke", "Smoke");
			node->attachObject(partSystem);
		}
		fire = true;
	}

	// PARICULE SIZE WHEN MODEL IS DISPLAYED
	//partSystem->setParameter("particle_width","3");
	//partSystem->setParameter("particle_height", "3");						

	return fire;
}

PlayState* PlayState::getSingletonPtr() {
	return msSingleton;
}

PlayState& PlayState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
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
	std::vector<std::vector<Ogre::SceneNode*>> &_grid, std::vector<Vehicle*> &vehicles) {

	std::vector<SceneNode*> nodeRow;
	std::stringstream nameNode;
	std::stringstream nameEntity;

	// Create grid
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

	//Create vehicles
	Vehicle* soldier = new Vehicle(1, 2, VehicleType::SOLDIER, Ogre::String(gridNode->getName()).append("_soldier"), *_sceneMgr);
	Vehicle* truck1 = new Vehicle(1, 3, VehicleType::TRUCK, Ogre::String(gridNode->getName()).append("_truck1"), *_sceneMgr);
	Vehicle* truck2 = new Vehicle(1, 3, VehicleType::TRUCK, Ogre::String(gridNode->getName()).append("_truck2"), *_sceneMgr);
	Vehicle* tank = new Vehicle(2, 2, VehicleType::TANK, Ogre::String(gridNode->getName()).append("_tank"), *_sceneMgr);
	Vehicle* missileLauncher = new Vehicle(2, 3, VehicleType::MISSILE_LAUNCHER, Ogre::String(gridNode->getName()).append("_missile"), *_sceneMgr);
	// ADD vehicles to gridNode
	gridNode->addChild(soldier->getNode());
	gridNode->addChild(truck1->getNode());
	gridNode->addChild(truck2->getNode());
	gridNode->addChild(tank->getNode());
	gridNode->addChild(missileLauncher->getNode());
	// ADD vehicles to vechicles vector
	vehicles.push_back(soldier);
	vehicles.push_back(truck1);
	vehicles.push_back(truck2);
	vehicles.push_back(tank);
	vehicles.push_back(missileLauncher);
}

// TODO COMMENT
bool PlayState::putVehicle(std::vector< std::vector< Ogre::SceneNode*>> &gridNode, Vehicle &vehicle, int x, int z, bool render)
{
	bool allow = true;
	int randOrientacion = rand() % 2;

	int heigh = 0;
	int width = 0;
	Ogre::Real yaw = 0;

	if (randOrientacion == 1)
	{
		heigh = vehicle.getHeigh();
		width = vehicle.getWidth();
		yaw = 90;
	}
	else
	{
		heigh = vehicle.getWidth();
		width = vehicle.getHeigh();
		yaw = 180;
	}

	std::vector<SceneNode*> vNodes;

	heigh += x;
	width += z;

	allow = heigh < gridNode.size() && width < gridNode.size() ? true : false;

	for (int i = x; i < heigh && allow; i++)
	{
		for (int j = z; j < width && allow; j++)
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
		vehicle.getNode()->yaw(Degree(yaw));
		vehicle.getNode()->setScale(0.28, 0.28, 0.2);
		if (render)
		{
			vehicle.render();
		}
		std::cout << nodeName.str() << std::endl;
		std::cout << "X:" << x << std::endl;
		std::cout << "Z:" << z << std::endl;
	}
	return allow;
}

bool PlayState::lose(std::vector<Vehicle*> &vehicles)
{
	bool lose = true;

	for (Vehicle *v : vehicles)
	{
		lose &= v->isDead();
	}
	return lose;
}