#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define SIZE_X 16
#define SIZE_Y 16




void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *p)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.6f, 0.8f), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram, false);
	sprite->setNumberAnimations(1);

	tileMapDispl = tileMapPos;
	player = p;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	velocitat = glm::vec2(4, -4);
}

void Ball::update(int deltaTime, vector<class Element>& elements)
{
	sprite->update(deltaTime);

	if (CollisionManager::instance().collisionBallMap(posBall, glm::ivec2(SIZE_X, SIZE_Y), map, velocitat));
	else if (CollisionManager::instance().collisionObjects(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), elements, velocitat));
	else if (CollisionManager::instance().collisionBallPlayer(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), player, velocitat));
	oldPosBall = posBall;
	posBall += velocitat;
	/*
	if (up) posBall.y -= 2;
	else posBall.y += 2;

	if (right) posBall.x += 2;
	else posBall.x -= 2;
	*/
	
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
	sprite->setPosition(glm::vec2(posBall.x+SIZE_X, posBall.y + SIZE_Y));
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
	oldPosBall = posBall;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}
