#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define SIZE_X 64
#define SIZE_Y 17



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.4f,0.9f) , glm::vec2(0.2, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.9f));
		sprite->addKeyframe(0, glm::vec2(0.f, 0.8f));
		sprite->addKeyframe(0, glm::vec2(0.f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	speed = 3;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	direction = glm::vec2(0, 0);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(!CollisionManager::instance().collisionPlayerMap(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(-1,0)*speed)) direction = glm::ivec2(-1, 0);
	}
	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(1, 0)*speed)) direction = glm::ivec2(1, 0);

	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(0, -1)*speed)) direction = glm::ivec2(0, -1);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (!CollisionManager::instance().collisionPlayerMap(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), map, glm::ivec2(0, 1)*speed)) direction = glm::ivec2(0, 1);
	}
	oldPosPlayer = posPlayer;
	posPlayer += direction*speed;

	/*else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_X, SIZE_Y), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}*/
	
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




