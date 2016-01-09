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
#define UNUSED_VARIABLE(x) (void)x

#include <vector>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <string>

#include "GameState.h"
#include "Vehicle.h"
#include "Enums.h"

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
public:
	PlayState() {}

	void enter();
	void exit();
	void pause();
	void resume();

	void keyPressed(const OIS::KeyEvent &e);
	void keyReleased(const OIS::KeyEvent &e);

	void mouseMoved(const OIS::MouseEvent &e);
	void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	
	static PlayState& getSingleton();
	static PlayState* getSingletonPtr();
	CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

	void createGUI();

private:
	Ogre::Root* _root;
	Ogre::SceneManager* _sceneMgr;
	Ogre::Viewport* _viewport;
	Ogre::Camera* _camera;


	GameFlow _gameflow = GameFlow::INIT;
	Ogre::SceneNode* _selectedNode;

	bool _exitGame = false;
	bool _retryGame = false;
	bool _resumeGame = false;

	// IA
	bool _iaTurn;
	Ogre::Real _contador;	
	int _xHitted = 0;
	int _zHitted = 0;
	std::stack<std::pair<int,int>> _shots;
	

	// CEGUI
	CEGUI::Window* playStateUI;
	CEGUI::Window* _gameOverUI;
	CEGUI::Window* _winUI;
	CEGUI::Window* _pauseUI;
	CEGUI::Window* _resume;
	CEGUI::Window* _exitPause;
	CEGUI::Window* _exitGameOver;
	CEGUI::Window* _retry;	
	CEGUI::Window* _scoreText;
	CEGUI::Window*	_nameText;
	CEGUI::Window* _save;
	CEGUI::Window* _turnText;
	
	// Time match for cegui GUI
	float _time = 0;
			

	//SCORE
	int _score = 0;
	//grid
	std::vector< std::vector< Ogre::SceneNode*>>  _playerGrid;
	std::vector< std::vector< Ogre::SceneNode*>>  _IAGrid;

	// vehicles
	std::vector<Vehicle*> _playerVehicles;
	std::vector<Vehicle*> _IAVehicles;

	//GUI
	bool resume(const CEGUI::EventArgs &e);
	bool quit(const CEGUI::EventArgs &e);
	bool retry(const CEGUI::EventArgs &e);
	bool save(const CEGUI::EventArgs &e);
	// interaction with the grid
	Ogre::SceneNode* getSceneNode(Ogre::Real const &x, Ogre::Real const &y, Ogre::uint32 mask);
	void createGrid(Ogre::SceneNode* node, Ogre::uint32 mask, std::vector<std::vector<Ogre::SceneNode*>>  &_grid, std::vector<Vehicle*> &vehicles);
	bool putVehicle(std::vector< std::vector< Ogre::SceneNode*>> &gridNode, Vehicle &vehicle, int x, int z, bool render);
	bool fire(Ogre::SceneNode* node);
	bool lose(std::vector<Vehicle*> &vehicles);
};
#endif
