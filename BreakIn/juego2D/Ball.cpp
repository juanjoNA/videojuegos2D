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

	start = false;
	tileMapDispl = tileMapPos;
	player = p;
	velocitat = glm::vec2(4, -4);
}

void Ball::update(int deltaTime, vector<class Element>& objInGame, int level)
{
	if (start) {
		int trans;
		sprite->update(deltaTime);
		switch (level) {
		case 0:
			trans = 0;
			break;
		case 1:
			trans = 464;
			break;
		case 2:
			trans = 928;
			break;
		}
		if (CollisionManager::instance().collisionBallMap(glm::ivec2(posBall.x, posBall.y + trans), oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), map, velocitat));
		else if (CollisionManager::instance().collisionObjects(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), objInGame, velocitat));
		else if (CollisionManager::instance().collisionBallPlayer(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), player, velocitat));
		oldPosBall = posBall;
		posBall += velocitat;
		sprite->setPosition(glm::vec2(posBall.x + SIZE_X, posBall.y + SIZE_Y));
	}
	else {
		if (player->isStarted()) start = true;
		sprite->setPosition(glm::vec2((player->getPosition().x + (player->getSize().x) / 2)+SIZE_X, (player->getPosition().y - player->getSize().y)+SIZE_Y));
	}
	
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

 glm::ivec2 Ball::position() {
	 return posBall;
}

 void Ball::begin() {
	 start = true;
 }

 void Ball::finish() {
	 start = false;
 }
