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
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X, SIZE_Y), glm::vec2(0.6f, 0.8f), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram, false);
	sprite->setNumberAnimations(1);

	start = false;
	tileMapDispl = tileMapPos;
	velocitat = glm::vec2(4, -4);
}

void Ball::update(int deltaTime, vector<class Element>& objInGame, Player *player)
{
	if (start) {
		bool rebote = false;

		sprite->update(deltaTime);

		rebote = CollisionManager::instance().collisionObjects(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), objInGame, velocitat);
		if (!rebote) rebote = CollisionManager::instance().collisionBallMap(glm::ivec2(posBall.x, posBall.y), glm::ivec2(oldPosBall.x, oldPosBall.y), glm::ivec2(SIZE_X, SIZE_Y), map, velocitat);
		if (!rebote) {
			if (objInGame.size() > 0) rebote = CollisionManager::instance().collisionBallPlayer(posBall, oldPosBall, glm::ivec2(SIZE_X, SIZE_Y), player, velocitat);
			else CollisionManager::instance().collisionBallPlayerVersus(posBall, glm::ivec2(SIZE_X, SIZE_Y), player, velocitat);
		}
		oldPosBall = posBall;
		posBall += velocitat;
		sprite->setPosition(glm::vec2(posBall.x + SIZE_X, posBall.y + SIZE_Y));
	}
	else {
		if (player->isStarted()) start = true;
		if (objInGame.size()>0) posBall = glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y);
		sprite->setPosition(glm::vec2(posBall.x + SIZE_X, posBall.y + SIZE_Y));
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

void Ball::setVelocity(glm::vec2 velocity) {
	velocitat = velocity;
}
