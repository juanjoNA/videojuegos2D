#ifndef _COLLISIONMANAGER_INCLUDE
#define _COLLISIONMANAGER_INCLUDE

#include "TileMap.h"
#include "Player.h"

class CollisionManager
{

public:
	CollisionManager() {}


	static CollisionManager &instance()
	{
		static CollisionManager CM;

		return CM;
	}
	//metodes de colisió
	bool collisionMap(glm::ivec2 &pos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const;
	bool collisionPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const;
	//bool collisionObjects(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, vector<Objects> objects) const;

private:

};


#endif // _PLAYER_INCLUDE


