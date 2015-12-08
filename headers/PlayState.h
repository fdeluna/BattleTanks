/*********************************************************************
* M�dulo 1. Curso de Experto en Desarrollo de Videojuegos
* Autor: David Vallejo Fern�ndez    David.Vallejo@uclm.es
*
* C�digo modificado a partir de Managing Game States with OGRE
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

#ifndef PlayState_H
#define PlayState_H

#define IA_GRID 1 << 0
#define PLAYER_GRIDNODE 2 << 0

#include <vector>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "GameState.h"
#include "Vehicle.h"


using namespace Ogre;
using namespace OIS;

class PlayState : public Singleton<PlayState>, public GameState
{
public:
	PlayState() {}

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
	static PlayState& getSingleton();
	static PlayState* getSingletonPtr();
	CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);
	
	void createGUI();

private:	
	Root* _root;
	SceneManager* _sceneMgr;
	Viewport* _viewport;
	Camera* _camera;


	GameFlow _gameflow = GameFlow::INIT;	
	SceneNode* _selectedNode;

	bool _exitGame;
	bool _iaTurn;

	// CEGUI
	CEGUI::OgreRenderer* renderer;
	CEGUI::Window* formatWin;

	// ??
	float _timeSinceLastFrame;
	float _turnTime;
	Ogre::Real _contador;
	

	//grid
	std::vector< std::vector< Ogre::SceneNode*>>  _playerGrid;
	std::vector< std::vector< Ogre::SceneNode*>>  _IAGrid;
	
	// vehicles
	std::vector<Vehicle*> _playerVehicles;
	std::vector<Vehicle*> _IAVehicles;


	// interaction with the grid
	SceneNode* getSceneNode(Ogre::Real const &x, Ogre::Real const &y, uint32 mask);
	void createGrid(SceneNode* node, uint32 mask, std::vector<std::vector<Ogre::SceneNode*>>  &_grid, std::vector<Vehicle*> &vehicles);		
	bool putVehicle(std::vector< std::vector< Ogre::SceneNode*>> &gridNode, Vehicle &vehicle, int x, int z, bool render);
	bool fire(SceneNode* node);
	bool lose(std::vector<Vehicle*> &vehicles);
};
#endif