#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size);
	void initVersus(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, glm::ivec2 & s, int player);
	void update(int deltaTime, int subnivel, int controls);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() { return posPlayer; }
	glm::vec2 getSize();
	int getSpeed() { return speed; }
	void setSpeed(int speed);
	//metodes de colisió
	bool collision(glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool isStarted() { return start; };
	void restart();

private:
	glm::ivec2 tileMapDispl, posPlayer, oldPosPlayer, direction, size;
	int speed;
	bool start;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE