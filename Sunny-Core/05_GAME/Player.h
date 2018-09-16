#pragma once

#include <SUNNY.h>

#include "graphics/Animation3D.h"

namespace game
{
	class Player
	{
	public:
		int hp;
		int id;
		bool isConnected;
		bool isReady;
		bool isMaster;
		short player_type;
	

		Animation3D* character;

	public:
		Player(int id) : id(id), hp(100) {};
	};
}
