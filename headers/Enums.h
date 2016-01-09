#ifndef Enum_H
#define Enum_H

enum class VehicleType
{
	SOLDIER = 1,
	TRUCK = 2,
	TANK = 3,
	MISSILE_LAUNCHER = 4
};

enum class GameFlow
{
	INIT,
	PLAY,
	GAMEOVER,
	PAUSE,
	WIN
};
#endif
