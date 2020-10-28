#ifndef _COLLISIONMANAGER_INCLUDE
#define _COLLISIONMANAGER_INCLUDE

#include "TileMap.h"
#include "Player.h"
#include "Component.h"

class CollisionManager
{

public:
	CollisionManager() {}


	static CollisionManager &instance()
	{
		static CollisionManager CM;

		return CM;
	}
	//metodes de colisi�
	bool collisionBallMap(glm::ivec2 &pos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const;
	bool collisionBallPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const;
	bool collisionPlayerMap(glm::ivec2 &pos, const glm::ivec2 &size, TileMap *tileMap, glm::ivec2 direction) const;
	//bool collisionObjects(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, vector<Component> components, glm::vec2 &velocitat) const;

private:

};


#endif // _PLAYER_INCLUDE


