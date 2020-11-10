#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *p);
	void update(int deltaTime, vector<class Element>& objInGame, int level);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 position();
	void begin();
	void finish();

private:
	glm::ivec2 tileMapDispl, posBall, oldPosBall;
	glm::vec2 velocitat;
	bool start;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;

};


#endif // _PLAYER_INCLUDE
