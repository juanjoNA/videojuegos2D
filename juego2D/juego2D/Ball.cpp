#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define SIZE_X 16
#define SIZE_Y 16




void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.6f, 0.8f), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	up = false;
	right = true;
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (map->collisionMoveLeft(posBall, glm::ivec2(SIZE_X, SIZE_Y)))
	{
		right = true;
	}
	if (map->collisionMoveRight(posBall, glm::ivec2(SIZE_X, SIZE_Y)))
	{
		right = false;
	}
	if (map->collisionMoveUp(posBall, glm::ivec2(SIZE_X, SIZE_Y)))
	{
		up = false;
	}
	if (map->collisionMoveDown(posBall, glm::ivec2(SIZE_X, SIZE_Y), &posBall.y))
	{
		up = true;
	}

	if (up) posBall.y -= 2;
	else posBall.y += 2;

	if (right) posBall.x += 2;
	else posBall.x -= 2;
	
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
	posBall.y = startY;
	}
	else
	{
	posBall.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
	if(jumpAngle > 90)
	bJumping = !map->collisionMoveDown(posBall, glm::ivec2(SIZE_X, SIZE_Y), &posBall.y);
	}
	}
	else
	{
	posBall.y += FALL_STEP;
	if(map->collisionMoveDown(posBall, glm::ivec2(SIZE_X, SIZE_Y), &posBall.y))
	{
	if(Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
	bJumping = true;
	jumpAngle = 0;
	startY = posBall.y;
	}
	}
	}*/

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2 &pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}
