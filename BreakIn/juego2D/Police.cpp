#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Police.h"
#include "Game.h"

#define SIZE_X 32
#define SIZE_Y 32



void Police::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.6f, 0.f), glm::vec2(0.05, 0.1), &spritesheet, &shaderProgram, true);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 10);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.05f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.1f, 0.f));

	sprite->setAnimationSpeed(1, 10);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.1f));
	sprite->addKeyframe(1, glm::vec2(0.05f, 0.1f));
	sprite->addKeyframe(1, glm::vec2(0.1f, 0.1f));

	//sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	speed = 1;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));

}

void Police::update(int deltaTime, Player *player)
{
	sprite->update(deltaTime);

	direction = moveToPlayer(player);
	posPolice += direction*speed;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
}

void Police::render()
{
	sprite->render();
}

void Police::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Police::setPosition(const glm::vec2 &pos)
{
	posPolice = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
}

glm::ivec2 Police::moveToPlayer(Player *player)
{
	glm::ivec2 move = glm::ivec2(0, 0);
	glm::vec2 posPlayer = player->getPosition();
	if (posPlayer.x < posPolice.x) {
		move.x = -1;
		if (sprite->getCurrentAnimation() != 1) sprite->changeAnimation(1);
	}
	else if (posPlayer.x > posPolice.x) {
		move.x = 1;
		if (sprite->getCurrentAnimation() != 0) sprite->changeAnimation(0);
	}

	if (posPlayer.y < posPolice.y) move.y = -1;
	else if (posPlayer.y > posPolice.y) move.y = 1;

	return move;
}

glm::ivec2 Police::getSize()
{
	return glm::ivec2(SIZE_X, SIZE_Y);
}

/*
bool Police::collision(glm::ivec2 &pos, Player *player, const glm::ivec2 &size) const
{
int xmin = pos.x;
int xmax = pos.x + size.x;
int ymin = pos.y;
int ymax = pos.y + size.y;
if (
(posPolice.x + SIZE_X >= xmin) &&
(xmax >= posPolice.x) &&
(posPolice.y + SIZE_Y >= ymin) &&
(ymax >= posPolice.y)
)
{
pos.y = pos.y - (ymax - posPolice.y);
return true;
}
return false;
}*/
