#include "PlayState.h"
#include "PauseState.h"
#include <stdio.h>
#include <boost/lexical_cast.hpp>

using namespace std;
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter() {

	_root = Ogre::Root::getSingletonPtr();
	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("IntroCamera");
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

	contador = 0;
	numeroSegundos = 0;
	//Variables y creacion de cegui
	_quit = false;
	_exitGame = false;
	createGUI();
}

void PlayState::exit() {
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause() {
}

void PlayState::resume() {
	// Se restaura el background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt) {

	_timeSinceLastFrame = evt.timeSinceLastFrame;
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
			_timeSinceLastFrame);
	int fps = 1.0/_timeSinceLastFrame;
	contador = contador + _timeSinceLastFrame;

	//Mostrar en el Text2 del layout el tiempo de partida
	std::stringstream tiempoPartida;
	tiempoPartida <<"Tiempo: " <<contador;
	formatWin->getChild("Text2")->setText(tiempoPartida.str());

	//Mostrar en el Text3 del layout los fps
		std::stringstream fpsInfo;
		fpsInfo <<"FPS: "<< fps;
		formatWin->getChild("Text3")->setText(fpsInfo.str());

	if (_quit)
		return false;
	return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e) {
	// Tecla p --> PauseState.

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
			static_cast<CEGUI::Key::Scan> (e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

	if (e.key == OIS::KC_P) {
		pushState(PauseState::getSingletonPtr());
	}

}

void PlayState::keyReleased(const OIS::KeyEvent &e) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(
			static_cast<CEGUI::Key::Scan> (e.key));

	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}

}

void PlayState::mouseMoved(const OIS::MouseEvent &e) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
			e.state.X.rel, e.state.Y.rel);

}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
			convertMouseButton(id));

	//Mostrar en el Text1 del layout la posicion del puntero
	std::stringstream posicion;
	posicion << "(" << e.state.X.rel << "," << e.state.Y.rel << ","
			<< e.state.Z.rel << ")";
	formatWin->getChild("Text1")->setText(posicion.str());

}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
			convertMouseButton(id));

}

PlayState*
PlayState::getSingletonPtr() {
	return msSingleton;
}

PlayState&
PlayState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

//FUNCION PROVISIONAL
//Funcion de salida de la aplicacion creada con CEGUI
bool PlayState::quit(const CEGUI::EventArgs &e) {
	_quit = true;
	return true;
}

//CEGUI
void PlayState::createGUI() {

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
	CEGUI::Window* exitButton = formatWin->getChild("ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&PlayState::quit, this));

	//Attaching buttons
	sheet->addChild(formatWin);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

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
