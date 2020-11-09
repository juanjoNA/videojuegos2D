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
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistance, vector<glm::vec2> &animations, char type);
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistance, vector<glm::vec2> &animations, char type, int value);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() { return posElement; }
	glm::vec2 getSize();
	int collision();
	bool isFinished();
	bool isActivated();
	int getResistance();
	int getType();
	int getValue() { return value; }
	void free();
	void activate();
	void executedConsequence();

private:
	glm::ivec2 tileMapDispl, posElement, size;
	int resistance, value;
	Texture spritesheet;
	Sprite sprite;
	int type; //BRICK = 0, MONEY = 1, ALARM = 2, KEY = 3;
	bool activateConsequence;
};


#endif // _PLAYER_INCLUDE


