#ifndef _POLICE_INCLUDE
#define _POLICE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Police
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, Player *player, int sublevel);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getSize();
	glm::ivec2 moveToPlayer(Player *player);
	void restart();
	bool isStarted() { return start; };
	void setStart(bool start);
	void setColision(bool colisionPlayer);
	bool catchPlayer() { return colPlayer; };


private:
	glm::ivec2 tileMapDispl, posIni, posPolice, direction;
	int speed;
	bool colPlayer, start;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE