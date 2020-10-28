#ifndef _Component_INCLUDE
#define _Component_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "CollisionManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Component
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistance, glm::vec2 keyframes[]);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() { return posComponent; }
	glm::vec2 getSize();
	void createSprite();
	void collision();

private:
	glm::ivec2 tileMapDispl, posComponent;
	int resistance;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _PLAYER_INCLUDE


