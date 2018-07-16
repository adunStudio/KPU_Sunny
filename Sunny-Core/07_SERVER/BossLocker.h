#pragma once

#include "SUNNY.h"
#include "../05_GAME/Player.h"

using namespace game;

class BossLocker
{
public:
	static unordered_map<int, Player*> players;

	static Player* player;

	static int id;

	static vector<Entity*> player_bullets;

	static PoolList* bulletList;
};