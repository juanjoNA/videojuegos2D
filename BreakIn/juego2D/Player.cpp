#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &s)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(s, glm::vec2(0.4f, 0.7f), glm::vec2(0.2, 0.1), &spritesheet, &shaderProgram, true);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.1f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.2f));


	sprite->changeAnimation(0);
	size = s;
	tileMapDispl = tileMapPos;
	speed = 4;
	start = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::initVersus(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &s, int player)
{
	spritesheet.loadFromFile("images/1v1Sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	if(player == 2) sprite = Sprite::createSprite(s, glm::vec2(0.0f, 0.0f), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram, true);
	else sprite = Sprite::createSprite(s, glm::vec2(0.0f, 0.5f), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram, true);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 10);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));


	sprite->changeAnimation(0);
	size = s;
	tileMapDispl = tileMapPos;
	speed = 4;
	start = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));


}

void Player::update(int deltaTime, int subnivel, int controls)
{
	sprite->update(deltaTime);
	direction = glm::vec2(0, 0);
	if (controls == 0) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (!start && subnivel == -1) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(-1, 0)*speed)) direction += glm::ivec2(-1, 0);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (!start && subnivel == -1) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(1, 0)*speed)) direction += glm::ivec2(1, 0);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (!start) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(0, -1)*speed)) direction += glm::ivec2(0, -1);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (!start) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(0, 1)*speed)) direction += glm::ivec2(0, 1);
		}
	}
	else {
		if (Game::instance().getKey('a'))
		{
			if (!start && subnivel == -2) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(-1, 0)*speed)) direction += glm::ivec2(-1, 0);
		}
		if (Game::instance().getKey('d'))
		{
			if (!start && subnivel == -2) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(1, 0)*speed)) direction += glm::ivec2(1, 0);
		}
		if (Game::instance().getKey('w'))
		{
			if (!start) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(0, -1)*speed)) direction += glm::ivec2(0, -1);
		}
		if (Game::instance().getKey('s'))
		{
			if (!start) start = true;
			if (!CollisionManager::instance().collisionPlayerMap(posPlayer, subnivel, size, map, glm::ivec2(0, 1)*speed)) direction += glm::ivec2(0, 1);
		}
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
	return size;
}

void Player::setSpeed(int s)
{
	speed = s;
}

void Player::restart()
{
	sprite->changeAnimation(0);
	start = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
