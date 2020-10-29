#ifndef _ELEMENT_INCLUDE
#define _ELEMENT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Element
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistance, vector<glm::vec2> &animations);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() { return posElement; }
	glm::vec2 getSize();
	int collision();

private:
	glm::ivec2 tileMapDispl, posElement;
	int resistance;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _PLAYER_INCLUDE


