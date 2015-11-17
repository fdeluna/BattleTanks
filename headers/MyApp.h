#include <Ogre.h>
#include "MyFrameListener.h"
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\Ogre\Renderer.h>

#define BOX 1 << 0  // Mascara para el escenario

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  CEGUI::OgreRenderer* renderer; 
  MyFrameListener* _framelistener;
  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createGUI();
  void initUI();
};
