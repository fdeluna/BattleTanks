#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter() {
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
	_camera = _sceneMgr->createCamera("IntroCamera");
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

	//Variables y creacion de cegui
	_exitGame = false;
	createGUI();
}

void IntroState::exit() {
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause() {
}

void IntroState::resume() {
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt) {

	_timeSinceLastFrame = evt.timeSinceLastFrame;
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
			_timeSinceLastFrame);
	/*
	 if (_play) {
	 changeState(PlayState::getSingletonPtr());
	 }
	 */
	return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void IntroState::keyPressed(const OIS::KeyEvent &e) {

	// Transición al siguiente estado.
	// Espacio --> PlayState
	/*
	 if (e.key == OIS::KC_SPACE) {
	 changeState(PlayState::getSingletonPtr());
	 }
	 */
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
			static_cast<CEGUI::Key::Scan> (e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

}

void IntroState::keyReleased(const OIS::KeyEvent &e) {

	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(
			static_cast<CEGUI::Key::Scan> (e.key));
}

void IntroState::mouseMoved(const OIS::MouseEvent &e) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
			e.state.X.rel, e.state.Y.rel);
}

void IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
			convertMouseButton(id));
}

void IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
			convertMouseButton(id));
}

IntroState*
IntroState::getSingletonPtr() {
	return msSingleton;
}

IntroState&
IntroState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

//FUNCION PROVISIONAL
//Ir a de intro a play
bool IntroState::play(const CEGUI::EventArgs &e) {

	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	changeState(PlayState::getSingletonPtr());

	return true;
}

//CEGUI
void IntroState::createGUI() {

	//CEGUI
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

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
			"DefaultWindow", "Ex1/Sheet");

	//Play button
	CEGUI::Window* playButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "Ex1/QuitButton");
	playButton->setText("Play");
	playButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	playButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0), CEGUI::UDim(0.2, 0)));

	playButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&IntroState::play, this));

	//Attaching buttons
	sheet->addChild(playButton);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

}
//Conversion mouse
CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id) {
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
