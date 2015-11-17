/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *********************************************************************/

#include "MyApp.h" 
#include <iostream>
#include <string>

MyApp::MyApp() {
	_sceneManager = NULL;
	_framelistener = NULL;
}

MyApp::~MyApp() {
}

int MyApp::start() {
	_root = new Ogre::Root();

	if (!_root->restoreConfig()) {
		_root->showConfigDialog();
		_root->saveConfig();
	}

	Ogre::RenderWindow* window = _root->initialise(true, "MyApp Example");
	_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

	Ogre::Camera* cam = _sceneManager->createCamera("MainCamera");
	cam->setPosition(Ogre::Vector3(13, 13, -40));
	cam->lookAt(Ogre::Vector3(13, 13, 0));
	cam->setNearClipDistance(5);
	cam->setFarClipDistance(10000);

	Ogre::Viewport* viewport = window->addViewport(cam);
	viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
	double width = viewport->getActualWidth();
	double height = viewport->getActualHeight();
	cam->setAspectRatio(width / height);


	loadResources();
	createScene();

	Ogre::SceneNode *node = _sceneManager->getSceneNode("SinbadNode");

	_framelistener = new MyFrameListener(window, cam, node);
	_root->addFrameListener(_framelistener);

	createGUI();

	_root->startRendering();

	return 0;
}

void MyApp::loadResources() {
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
	Ogre::String sectionstr, typestr, datastr;
	while (sI.hasMoreElements()) {
		sectionstr = sI.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typestr = i->first;    datastr = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation
				(datastr, typestr, sectionstr);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {

	Ogre::SceneNode* node1 = _sceneManager->createSceneNode("SinbadNode");
	//node1->setPosition(-1, -1, 0);
	std::stringstream nameNode;
	std::stringstream nameEntity;	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			nameNode << "node_"<<i <<'_' << j;
			nameEntity<<"entity_" << i << '_' << j;
			Ogre::Entity* ent1 = _sceneManager->createEntity(nameEntity.str(), "cube.mesh");			
			ent1->setQueryFlags(BOX);
			Ogre::SceneNode* node = _sceneManager->createSceneNode(nameNode.str());		
			node->attachObject(ent1);			
			node->setPosition(Ogre::Vector3(i * 3, j * 3, 0));
			node1->addChild(node);
		}
	}	

	//Ogre::Entity* ent1 = _sceneManager->createEntity("Entity 1", "cube.mesh");
	//Ogre::SceneNode* node = _sceneManager->createSceneNode("Node 1");
	//node->attachObject(ent1);
	//node->setPosition(Ogre::Vector3(0, 0, 0));

	//Ogre::Entity* ent2 = _sceneManager->createEntity("Entity 2", "cube.mesh");
	//Ogre::SceneNode* nod2 = _sceneManager->createSceneNode("Node 2");
	//nod2->attachObject(ent2);
	//nod2->setPosition(Ogre::Vector3(0, 3, 0));

	//Ogre::Entity* ent3 = _sceneManager->createEntity("Entity 3", "cube.mesh");
	//Ogre::SceneNode* node3 = _sceneManager->createSceneNode("Node 3");
	//node3->attachObject(ent3);
	//node3->setPosition(Ogre::Vector3(0, 6, 0));





	//node1->addChild(node);
	//node1->addChild(nod2);
	//node1->addChild(node3);


	_sceneManager->getRootSceneNode()->addChild(node1);


	// add two lights
	Ogre::Light* light1 = _sceneManager->createLight();
	light1->setType(Ogre::Light::LT_POINT);
	light1->setPosition(0, 0, 0);
	light1->setSpecularColour(Ogre::ColourValue::White);
	node1->attachObject(light1);

	Ogre::Light* light2 = _sceneManager->createLight();
	light2->setType(Ogre::Light::LT_POINT);
	light2->setPosition(10, -100, -100);
	light2->setSpecularColour(Ogre::ColourValue::White);
	node1->attachObject(light2);
}

void MyApp::createGUI()
{
	//CEGUI
	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-12");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "Ex1/Sheet");

	//Quit button
	CEGUI::Window* quitButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Ex1/QuitButton");
	quitButton->setText("Quit");
	quitButton->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0), CEGUI::UDim(0.2, 0)));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&MyFrameListener::quit,
		_framelistener));
	//Attaching buttons
	sheet->addChild(quitButton);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}
