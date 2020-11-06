#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Element.h"
#include "Game.h"

#define SIZE_X 32
#define SIZE_Y 16


enum elementType {
	BRICK, MONEY, ALARM, KEY
};

void Element::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, vector<glm::vec2> &animations, char letter)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if(letter != 'A') sprite = Sprite(size, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram, false);
	else sprite = Sprite(size, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram, true);
	sprite.setNumberAnimations(resistencia);

	if (resistencia > 1)
	{
		for (int i = 0; i < resistencia; i++) {
			sprite.setAnimationSpeed(i, 5);
			sprite.addKeyframe(i, animations[i]);
		}
		sprite.changeAnimation(0);
	}
	else {
		if (!animations.empty()) {
			if(animations.size() > 4) sprite.setAnimationSpeed(0, 25);
			else if(letter != 'A') sprite.setAnimationSpeed(0, 8);
			else sprite.setAnimationSpeed(0, 10);
			for (int i = 1; i < animations.size(); i++) {
				sprite.addKeyframe(0, animations[i]);
			}
		}
	}
	resistance = resistencia;
	tileMapDispl = tileMapPos;
	if (letter == 'B') type = BRICK;
	else if (letter == 'M') type = MONEY;
	else if (letter == 'A') type = ALARM;
	else type = KEY;

}

void Element::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &size, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, vector<glm::vec2> &animations, char letter, int val)
{
	init(tileMapPos, shaderProgram, size, posInSpritesheet, sizeInSpritesheet, resistencia, animations, letter);
	value = val;
}

void Element::update(int deltaTime)
{
	sprite.update(deltaTime);

}

void Element::render()
{
	sprite.render();
}


void Element::setPosition(const glm::vec2 &pos)
{
	posElement = pos;
	sprite.setPosition(glm::vec2(float(tileMapDispl.x + posElement.x), float(tileMapDispl.y + posElement.y)));
}

glm::vec2 Element::getSize()
{
	return glm::vec2(SIZE_X, SIZE_Y);
}

int Element::collision()
{
	resistance--;
	if (resistance > 0) {
		int animId = sprite.getCurrentAnimation();
		sprite.changeAnimation(animId + 1);
	}
	else if(resistance == 0 && type != BRICK) {
		sprite.changeAnimation(0);
	}
	else {

	}

	return resistance;
}

bool Element::isFinished() {
	return sprite.isFinished();
}

int Element::getResistance() {
	return resistance;
}

int Element::getType() {
	return type;
}

void Element::free() {
	sprite.free();
}