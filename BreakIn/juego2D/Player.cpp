#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define SIZE_X 64
#define SIZE_Y 16



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.4f,0.7f) , glm::vec2(0.2, 0.1), &spritesheet, &shaderProgram, true);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.f, 0.1f));
		sprite->addKeyframe(0, glm::vec2(0.f, 0.2f));
		

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	speed = 4;
	start = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, int subnivel)
{
	sprite->update(deltaTime);
	direction = glm::vec2(0, 0);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(-1,0)*speed)) direction += glm::ivec2(-1, 0);
	}
	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(1, 0)*speed)) direction += glm::ivec2(1, 0);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (!start) start = true;
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(0, -1)*speed)) direction += glm::ivec2(0, -1);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (!start) start = true;
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(0, 1)*speed)) direction += glm::ivec2(0, 1);
	}
	oldPosPlayer = posPlayer;
	posPlayer += direction*speed;
		
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getSize()
{
	return glm::vec2(SIZE_X, SIZE_Y);
}

bool Player::collision(glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int xmin = pos.x;
	int xmax = pos.x + size.x;
	int ymin = pos.y;
	int ymax = pos.y + size.y;

	if (
		(posPlayer.x + SIZE_X >= xmin) &&
		(xmax >= posPlayer.x) &&
		(posPlayer.y + SIZE_Y >= ymin) &&
		(ymax >= posPlayer.y)
		)
	{
		pos.y = pos.y - (ymax - posPlayer.y);
		return true;
	}

	return false;
}