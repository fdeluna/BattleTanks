#include <Ogre.h>
#include <OIS.h>
#include <CEGUI.h>

class MyFrameListener : public Ogre::FrameListener, OIS::KeyListener, OIS::MouseListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Ogre::Camera* _camera;
  Ogre::SceneNode *_node;

  bool keyPressed(const OIS::KeyEvent& evt);
  bool keyReleased(const OIS::KeyEvent& evt);
  bool mouseMoved(const OIS::MouseEvent& evt);
  bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

  bool _quit;
  float _timeSinceLastFrame;

  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, 
		  Ogre::SceneNode* node);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);  
  bool quit(const CEGUI::EventArgs &e);
};
