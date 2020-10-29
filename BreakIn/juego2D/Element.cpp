#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Element.h"
#include "Game.h"

#define SIZE_X 32
#define SIZE_Y 16


enum elementType{
	BRICK, MONEY, KEY
};

void Element::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, vector<glm::vec2> &animations)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(resistencia);

	if(resistencia > 1) 
	{
		for (int i = 0; i < resistencia; i++) {
			sprite->setAnimationSpeed(i, 5);
			sprite->addKeyframe(i, animations[i]);
		}
		sprite->changeAnimation(0);
	}
	else {
		if (!animations.empty()) {
			if(animations.size() > 4) sprite->setAnimationSpeed(0, 1);
			else sprite->setAnimationSpeed(0, 8);
			for (int i = 1; i < animations.size(); i++) {
				sprite->addKeyframe(0, animations[i]);
			}
		}
	}
	resistance = resistencia;
	
	tileMapDispl = tileMapPos;
}

void Element::update(int deltaTime)
{
	sprite->update(deltaTime);
	
}

void Element::render()
{
	sprite->render();
}


void Element::setPosition(const glm::vec2 &pos)
{
	posElement = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posElement.x), float(tileMapDispl.y + posElement.y)));
}

glm::vec2 Element::getSize()
{
	return glm::vec2(SIZE_X, SIZE_Y);
}

int Element::collision()
{
	resistance--;
	if (resistance > 0) {
		int animId = sprite->getCurrentAnimation();
		sprite->changeAnimation(animId + 1);
	}
	if(resistance == 0) {
		sprite->changeAnimation(0);
		//sprite->free();
	}
	
	return resistance;
}






