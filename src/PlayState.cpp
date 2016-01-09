#include "PlayState.h"
#include "IntroState.h"
#include <fstream>
#include <string>
using namespace std;

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;


void PlayState::enter() {

	Ogre::MovableObject::setDefaultQueryFlags(0);

	// Initialize Game flow
	_gameflow = GameFlow::INIT;
	_root = Ogre::Root::getSingletonPtr();

	if (_root->hasSceneManager("PlayState") && _sceneMgr->hasCamera(
		"IntroCamera")) {
		_sceneMgr = _root->getSceneManager("PlayState");
		_camera = _sceneMgr->getCamera("IntroCamera");
	}
	else {
		_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "PlayState");
		// set camera
		_camera = _sceneMgr->createCamera("IntroCamera");
	}
	_camera->setPosition(0, 45, -22);
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(1);
	_camera->setFarClipDistance(10000);

	// add viewport
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	double width = _viewport->getActualWidth();
	double height = _viewport->getActualHeight();
	_camera->setAspectRatio(width / height);

	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Ogre::Light* light = _sceneMgr->createLight("Luz1");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(0, 150, 250);
	_sceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("plane1",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
		200, 200, 1, 1, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);
	Ogre::SceneNode* groundNode = _sceneMgr->createSceneNode("ground");
	Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
	groundEnt->setMaterialName("Ground");
	groundNode->attachObject(groundEnt);
	_sceneMgr->getRootSceneNode()->addChild(groundNode);
	_exitGame = false;
	_retryGame = false;
	_resumeGame = false;

	_time = 0;
	_score = 0;
	
	
	srand(time(NULL));	
	createGUI();
}

void PlayState::exit() {
	// Clean playState	
	_playerGrid.clear();
	_IAGrid.clear();
	_playerVehicles.clear();
	_IAVehicles.clear();
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
	while (!_shots.empty())
	{
		_shots.pop();
	}

}

void PlayState::pause() {}

void PlayState::resume() {}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
		evt.timeSinceLastFrame);

	std::stringstream matchTime;
	matchTime << "       " << (int)_time;
	playStateUI->getChild("GameTime")->setText(
		"[colour='FF000000']" + matchTime.str());

	switch (_gameflow) {
	case GameFlow::INIT: {
		// Player grid
		Ogre::SceneNode* node1 = _sceneMgr->createSceneNode("PlayerGrid");

		node1->setPosition(23, 0, 10);
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
		node2->setPosition(-3, 0, 10);
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
	case GameFlow::PLAY: {		
		_time += evt.timeSinceLastFrame;
		if (_iaTurn)
		{
			_turnText->setText("IA TURN");
			_contador += evt.timeSinceLastFrame;
			bool shotted = false;
			if (_contador >= 1) {
				try {
					if (!_shots.empty())
					{
						while (!shotted && !_shots.empty())
						{
							_xHitted = _shots.top().first;
							_zHitted = _shots.top().second;
							_shots.pop();
							shotted = fire(_playerGrid[_xHitted][_zHitted]);
						}
					}
					else if (!shotted)
					{
						_xHitted = rand() % 10;
						_zHitted = rand() % 10;

						while (!fire(_playerGrid[_xHitted][_zHitted]))
						{
							_xHitted = rand() % 10;
							_zHitted = rand() % 10;
						}
					}
				}
				catch (Ogre::Exception& e) {
					UNUSED_VARIABLE(e);
					_iaTurn = true;
				}
				_contador = 0;
				_iaTurn = false;
			}
		}
		else
		{
			_turnText->setText("PLAYER TURN");
		}
	}
						 break;
	case GameFlow::GAMEOVER: {
		_gameOverUI->setVisible(true);
	}
							 break;
	case GameFlow::PAUSE: {
		_pauseUI->setVisible(true);
	}
						  break;

	case GameFlow::WIN: {
		_winUI->setVisible(true);
		_nameText->setVisible(true);

	}
						break;
	}
	return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_gameflow == GameFlow::GAMEOVER || _gameflow == GameFlow::WIN) {
		if (_retryGame) {
			changeState(PlayState::getSingletonPtr());
		}
	}
	if (_gameflow == GameFlow::PAUSE) {
		if (_resumeGame) {
			_pauseUI->setVisible(false);
			_gameflow = GameFlow::PLAY;
			_resumeGame = false;
		}

	}

	if (_gameflow == GameFlow::PLAY) {
		if (lose(_playerVehicles)) {
			std::cout << "IA WINS";
			_gameflow = GameFlow::GAMEOVER;
		}
		if (lose(_IAVehicles)) {
			std::cout << "Players WINS";
			_gameflow = GameFlow::WIN;
		}
	}

	if (_exitGame) {

		CEGUI::WindowManager::getSingleton().destroyAllWindows();
		changeState(IntroState::getSingletonPtr());

	}

	return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
		static_cast<CEGUI::Key::Scan> (e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

}

void PlayState::keyReleased(const OIS::KeyEvent &e) {
	if (e.key == OIS::KC_ESCAPE) {
		_gameflow = GameFlow::PAUSE;
	}
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(
		static_cast<CEGUI::Key::Scan> (e.key));
}

void PlayState::mouseMoved(const OIS::MouseEvent &e) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
		e.state.X.rel, e.state.Y.rel);

	if (!_iaTurn) {
		Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
		Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;

		std::stringstream nombre;
		if (_selectedNode) {
			nombre.str("");
			nombre << _selectedNode->getName();
		}

		Ogre::SceneNode* aux = getSceneNode(normX, normY, IA_GRID);

		if (!_selectedNode && aux) {
			_selectedNode = aux;
		}

		if (_selectedNode != aux) {
			static_cast<Ogre::Entity*> (_selectedNode->getAttachedObject(0))->setMaterialName(
				"Cube1");
			_selectedNode = aux;
		}
		else if (_selectedNode) {
			static_cast<Ogre::Entity*> (_selectedNode->getAttachedObject(0))->setMaterialName(
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

	if (!_iaTurn) {
		if (convertMouseButton(id) == CEGUI::LeftButton) {
			std::stringstream posicion2;
			posicion2 << "(" << e.state.X.abs << "," << e.state.Y.abs << ","
				<< e.state.Z.abs << ")";
			Ogre::Real normX = Ogre::Real(e.state.X.abs) / e.state.width;
			Ogre::Real normY = Ogre::Real(e.state.Y.abs) / e.state.height;
			Ogre::SceneNode* aux = getSceneNode(normX, normY, IA_GRID);
			_iaTurn = fire(aux);
		}
	}
}

bool PlayState::fire(Ogre::SceneNode* node) {
	bool fire = false;
	_selectedNode = NULL;
	if (node && static_cast<Ogre::Entity*> (node->getAttachedObject(0))->getQueryFlags())
	{
		static_cast<Ogre::Entity*> (node->getAttachedObject(0))->setQueryFlags(NULL);
		static_cast<Ogre::Entity*> (node->getAttachedObject(0))->setMaterialName(
			"Crater");

		Vehicle* vAux = NULL;
		try {
			vAux = Ogre::any_cast<Vehicle*>(
				node->getUserObjectBindings().getUserAny());
		}
		catch (Ogre::Exception& e) {
			UNUSED_VARIABLE(e);
		}

		if (vAux) {
			Ogre::ParticleSystem* partSystem = _sceneMgr->createParticleSystem(
				node->getName() + "_Smoke", "Smoke");
			node->attachObject(partSystem);
			vAux->addGridNode(node);
			vAux->decreaseHP();
			//SCORE
			if (!_iaTurn) {
				_score = _score + 10;
			}
			else
			{
				if (_xHitted + 1 < 10)
				{
					_shots.push(make_pair(_xHitted + 1, _zHitted));
				}

				if (_xHitted - 1 >= 0){
					_shots.push(make_pair(_xHitted - 1, _zHitted));
				}

				if (_zHitted + 1 < 10)
				{
					_shots.push(make_pair(_xHitted, _zHitted + 1));
				}

				if (_zHitted - 1 >= 0)
				{
					_shots.push(make_pair(_xHitted, _zHitted - 1));
				}
			}

			if (vAux->isDead()) {
				vAux->render();
				//SCORE
				if (!_iaTurn) {
					switch (vAux->getType())
					{
					case VehicleType::SOLDIER:
						_score += 30;
						break;
					case VehicleType::TRUCK:
						_score += 50;
						break;
					case VehicleType::TANK:
						_score += 70;
						break;
					default:
						_score += 100;
						break;
					}
				}
			}
		}
		else {
			if (!_iaTurn && _score > 0) {
				_score = _score - 1;
			}
		}
		//SCORE
		if (!_iaTurn) {
			std::stringstream scoreString;
			scoreString << _score;
			playStateUI->getChild("ScoreText")->setText(scoreString.str());
			_scoreText->setText(scoreString.str());
		}
		fire = true;
	}
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
bool PlayState::quit(const CEGUI::EventArgs &e) {
	_exitGame = true;
	return true;
}
bool PlayState::resume(const CEGUI::EventArgs &e) {
	_resumeGame = true;
	return true;
}
bool PlayState::retry(const CEGUI::EventArgs &e) {
	_retryGame = true;
	return true;
}
bool PlayState::save(const CEGUI::EventArgs &e) {
	//ReadScores
	std::ofstream _scoresTXT;
	_scoresTXT.open("scores.txt", std::ofstream::app);

	std::stringstream txt;
	txt << _nameText->getText() << " / " << _score << "\n";

	_scoresTXT << txt.str();

	_exitGame = true;
	return true;
}
void PlayState::createGUI() {

	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
		"TaharezLook/MouseArrow");

	// load all the fonts 
	CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
		"DefaultWindow", "Sheet");

	//Config Window	
	playStateUI = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
		"playLayout.layout");

	playStateUI->getChild("GameTime")->setText(
		"[colour='FF00FF00'] asdf [padding='l:20 t:15 r:20 b:15']");

	_pauseUI = playStateUI->getChild("FondoPause");
	_gameOverUI = playStateUI->getChild("FondoGameOver");
	_winUI = playStateUI->getChild("FondoWin");
	_turnText = playStateUI->getChild("Turn");

	_resume = _pauseUI->getChild("Resume");
	_exitPause = _pauseUI->getChild("Exit");
	_retry = _gameOverUI->getChild("Retry");
	_exitGameOver = _gameOverUI->getChild("Exit");	
	_scoreText = _winUI->getChild("LabelScore");
	_nameText = _winUI->getChild("NameText");
	_save = _winUI->getChild("Save");

	_resume->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PlayState::resume, this));
	_exitPause->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PlayState::quit, this));
	_retry->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PlayState::retry, this));
	_exitGameOver->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PlayState::quit, this));

	_save->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PlayState::save, this));

	_winUI->setVisible(false);
	_pauseUI->setVisible(false);
	_gameOverUI->setVisible(false);
	_nameText->setVisible(false);

	sheet->addChild(playStateUI);

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
Ogre::SceneNode* PlayState::getSceneNode(Ogre::Real const &x, Ogre::Real const &y,
	Ogre::uint32 mask) {

	// Ray to mouse position	
	Ogre::RaySceneQuery* _raySceneQuery = _sceneMgr->createRayQuery(
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
void PlayState::createGrid(Ogre::SceneNode* gridNode, Ogre::uint32 mask,
	std::vector<std::vector<Ogre::SceneNode*>> &_grid,
	std::vector<Vehicle*> &vehicles) {

	std::vector<Ogre::SceneNode*> nodeRow;
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
			ent1->setMaterialName("Cube1");
			// Add mask to select node
			ent1->setQueryFlags(mask);
			Ogre::SceneNode* node = _sceneMgr->createSceneNode(nameNode.str());
			node->attachObject(ent1);
			node->setPosition(j * -2.15, 0, i * -2.15);
			node->setScale(4.5, 1.5, 4.5);
			node->getUserObjectBindings().setUserAny(Ogre::Any(NULL));
			// Attach child nodes to gridNode			
			nodeRow.push_back(node);
			gridNode->addChild(node);
		}
		_grid.push_back(nodeRow);
		nodeRow.clear();
	}

	//Create vehicles
	Vehicle* soldier1 = new Vehicle(1, 2, VehicleType::SOLDIER,
		Ogre::String(gridNode->getName()).append("_soldier1"), *_sceneMgr);
	Vehicle* soldier2 = new Vehicle(1, 2, VehicleType::SOLDIER,
		Ogre::String(gridNode->getName()).append("_soldier2"), *_sceneMgr);
	Vehicle* truck1 = new Vehicle(1, 3, VehicleType::TRUCK,
		Ogre::String(gridNode->getName()).append("_truck1"), *_sceneMgr);
	Vehicle* truck2 = new Vehicle(1, 3, VehicleType::TRUCK,
		Ogre::String(gridNode->getName()).append("_truck2"), *_sceneMgr);
	Vehicle* tank = new Vehicle(2, 2, VehicleType::TANK,
		Ogre::String(gridNode->getName()).append("_tank"), *_sceneMgr);
	Vehicle* missileLauncher = new Vehicle(2, 3, VehicleType::MISSILE_LAUNCHER,
		Ogre::String(gridNode->getName()).append("_missile"), *_sceneMgr);
	// ADD vehicles to gridNode
	gridNode->addChild(soldier1->getNode());
	gridNode->addChild(soldier2->getNode());
	gridNode->addChild(truck1->getNode());
	gridNode->addChild(truck2->getNode());
	gridNode->addChild(tank->getNode());
	gridNode->addChild(missileLauncher->getNode());
	// ADD vehicles to vechicles vector
	vehicles.push_back(soldier1);
	vehicles.push_back(soldier2);
	vehicles.push_back(truck1);
	vehicles.push_back(truck2);
	vehicles.push_back(tank);
	vehicles.push_back(missileLauncher);
}

bool PlayState::putVehicle(
	std::vector<std::vector<Ogre::SceneNode*>> &gridNode, Vehicle &vehicle,
	int x, int z, bool render) {
	bool allow = true;
	int randOrientacion = rand() % 2;

	int heigh = 0;
	int width = 0;
	Ogre::Real yaw = 0;

	if (randOrientacion == 1) {
		heigh = vehicle.getHeigh();
		width = vehicle.getWidth();
		yaw = 90;
	}
	else {
		heigh = vehicle.getWidth();
		width = vehicle.getHeigh();
		yaw = 180;
	}

	std::vector<Ogre::SceneNode*> vNodes;

	heigh += x;
	width += z;

	allow = heigh < gridNode.size() && width < gridNode.size() ? true : false;

	for (int i = x; i < heigh && allow; i++) {
		for (int j = z; j < width && allow; j++) {
			Vehicle* vAux = NULL;
			try {
				vAux = Ogre::any_cast<Vehicle*>(
					gridNode[i][j]->getUserObjectBindings().getUserAny());
			}
			catch (Ogre::Exception& e) {				
				UNUSED_VARIABLE(e);
			}

			if (vAux) {
				allow = false;
				break;
			}
			else {
				gridNode[i][j]->getUserObjectBindings().setUserAny(
					Ogre::Any(&vehicle));
				vNodes.push_back(gridNode[i][j]);
			}
		}
	}

	if (!allow) {
		for (Ogre::SceneNode* node : vNodes)
		{
			node->getUserObjectBindings().setUserAny(Ogre::Any(NULL));
		}
	}
	else
	{
		vehicle.setPosition(gridNode[x][z]->getPosition(), yaw);
		if (render)

			vehicle.render();
	}
	return allow;
}

bool PlayState::lose(std::vector<Vehicle*> &vehicles) {
	bool lose = true;

	for (Vehicle *v : vehicles)
	{
		lose &= v->isDead();
	}
	return lose;
}