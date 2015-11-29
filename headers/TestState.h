/*********************************************************************
* Módulo 1. Curso de Experto en Desarrollo de Videojuegos
* Autor: David Vallejo Fernández    David.Vallejo@uclm.es
*
* Código modificado a partir de Managing Game States with OGRE
* http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
* Inspirado en Managing Game States in C++
* http://gamedevgeek.com/tutorials/managing-game-states-in-c/
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

#ifndef TestState_H
#define TestState_H

#define GRIDNODE 1 << 0
#define GRIDHEIGH 10
#define GRIDWIDTH 10

#include <vector>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules/Ogre/Renderer.h>

#include "GameState.h"


using namespace Ogre;
using namespace OIS;



class TestState : public Singleton<TestState>, public GameState
{
public:
	TestState() {}

	void enter();
	void exit();
	void pause();
	void resume();

	void keyPressed(const KeyEvent &e);
	void keyReleased(const KeyEvent &e);

	void mouseMoved(const MouseEvent &e);
	void mousePressed(const MouseEvent &e, MouseButtonID id);
	void mouseReleased(const MouseEvent &e, MouseButtonID id);

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);

	// Heredados de Ogre::Singleton.
	static TestState& getSingleton();
	static TestState* getSingletonPtr();
	CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);



	void createGUI();

private:
	Root* _root;
	SceneManager* _sceneMgr;
	Viewport* _viewport;
	Camera* _camera;

	//RaySceneQuery* _raySceneQuery;
	//Ray _mouseRay;
	SceneNode* _selectedNode;

	bool _exitGame;

	// CEGUI
	CEGUI::OgreRenderer* renderer;
	CEGUI::Window* formatWin;


	float _timeSinceLastFrame;
	float contador;
	int numeroSegundos;
	
	std::vector<SceneNode*>  _playerGrid;
	std::vector<SceneNode*>  _enemyGrid;	

	SceneNode* getSceneNode(Ogre::Real const &x, Ogre::Real const &y, uint32 mask);	
	void createGrid(SceneNode* node, uint32 mask, std::vector<SceneNode*>  &_grid);
};

#endif