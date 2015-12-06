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

#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "PlayState.h"

#include <iostream>

using namespace std;

int main() {

	GameManager* game = new GameManager();
	PlayState* playState = new PlayState();

	UNUSED_VARIABLE(playState);


	try
	{
		// Inicializa el juego y transici�n al primer estado.
		game->start(PlayState::getSingletonPtr());
	}
	catch (Ogre::Exception& e)
	{
		cerr << "Excepci�n detectada: " << e.getFullDescription();
	}

	delete game;

	return 0;
}
