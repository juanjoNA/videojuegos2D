#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Component.h"
#include "Game.h"

#define SIZE_X 32
#define SIZE_Y 16



void Component::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, glm::vec2 animations[])
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
	resistance = resistencia;
	
	tileMapDispl = tileMapPos;
}

void Component::update(int deltaTime)
{
	sprite->update(deltaTime);
	
}

void Component::render()
{
	sprite->render();
}


void Component::setPosition(const glm::vec2 &pos)
{
	posComponent = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posComponent.x), float(tileMapDispl.y + posComponent.y)));
}

glm::vec2 Component::getSize()
{
	return glm::vec2(SIZE_X, SIZE_Y);
}






