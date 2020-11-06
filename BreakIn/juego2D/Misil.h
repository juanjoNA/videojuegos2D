#ifndef _MISIL_INCLUDE
#define _MISIL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Misil
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, Player *p);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() { return posMisil; }
	glm::vec2 getSize();
	int collision();

private:
	bool finish;
	glm::ivec2 tileMapDispl, posMisil;
	int velocidad;
	Texture spritesheet;
	Sprite sprite;
	Player *player;
};


#endif // _PLAYER_INCLUDE


