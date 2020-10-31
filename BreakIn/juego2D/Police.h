#ifndef _POLICE_INCLUDE
#define _POLICE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Police
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, Player *player);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getSize();
	glm::ivec2 moveToPlayer(Player *player);
	//metodes de colisió
	bool collision(glm::ivec2 &pos, Player *player, glm::ivec2 &size) const;
	/*bool collisionLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMiddle(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	*/

private:
	glm::ivec2 tileMapDispl, posPolice, direction;
	int speed;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE