#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *p);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posBall, oldPosBall;
	glm::vec2 velocitat;
	int startY;
	float bounceAngle;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;

};


#endif // _PLAYER_INCLUDE
