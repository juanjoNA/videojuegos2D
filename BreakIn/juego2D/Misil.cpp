#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Misil.h"
#include "Game.h"
#include "Player.h"

#define SIZE_X 32
#define SIZE_Y 16

void Misil::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, Player *p)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite(size, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram, false);
	sprite.setNumberAnimations(1);

	player = p;
	velocidad = 4;
	tileMapDispl = tileMapPos;
}

void Misil::update(int deltaTime)
{
	sprite.update(deltaTime);
	int colision = CollisionManager::instance().collisionMisil(posMisil, glm::ivec2(SIZE_X, SIZE_Y), player);
	switch (colision) {
		case 0:
			posMisil.y += velocidad;
			break;
		case 1:
			break;
		case 2: 
			sprite.free();
			finish = true;
			break;
	}
	sprite.setPosition(glm::vec2(float(tileMapDispl.x + posMisil.x), float(tileMapDispl.y + posMisil.y)));
}

void Misil::render()
{
	sprite.render();
}


void Misil::setPosition(const glm::vec2 &pos)
{
	posMisil = pos;
	sprite.setPosition(glm::vec2(float(tileMapDispl.x + posMisil.x), float(tileMapDispl.y + posMisil.y)));
}

glm::vec2 Misil::getSize()
{
	return glm::vec2(SIZE_X, SIZE_Y);
}
