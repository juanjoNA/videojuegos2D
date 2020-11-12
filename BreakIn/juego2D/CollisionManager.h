#ifndef _COLLISIONMANAGER_INCLUDE
#define _COLLISIONMANAGER_INCLUDE

#include "Element.h"
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
	//metodes de colision
	bool collisionBallMap(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const;
	bool collisionBallPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const;
	bool collisionBallPlayerVersus(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat, int numPlayer) const;
	bool collisionPlayerMap(glm::ivec2 &pos, int subnivel, const glm::ivec2 &size, TileMap *tileMap, glm::ivec2 direction) const;
	bool collisionObjects(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, vector<class Element>& elements, glm::vec2 &velocitat) const;
	int collisionMisil(glm::ivec2 &posMisil, const glm::ivec2 &sizeMisil, Player *player) const;
	bool collisionPolice(glm::ivec2 &pos, Player *player, glm::ivec2 &size);
	bool hayColision(glm::ivec2 &pos1, const glm::ivec2 &size1, const glm::ivec2 &pos2, const glm::ivec2 &size2) const;

private:

};


#endif // _PLAYER_INCLUDE