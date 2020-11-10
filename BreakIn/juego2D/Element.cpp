#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Element.h"
#include "Game.h"

enum elementType {
	BRICK, MONEY, ALARM, KEY, DOOR
};

void Element::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &s, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, vector<glm::vec2> &animations, char letter)
{
	spritesheet.loadFromFile("images/spriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if(letter != 'A') sprite = Sprite(s, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram, false);
	else sprite = Sprite(s, posInSpritesheet, sizeInSpritesheet, &spritesheet, &shaderProgram, true);
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

	
	activateConsequence = false;
	resistance = resistencia;
	tileMapDispl = tileMapPos;
	size = s;

	switch (letter) {
		case 'B':
			type = BRICK;
			break;
		case 'M':
			type = MONEY;
			break;
		case 'A':
			type = ALARM;
			break;
		case 'K':
			type = KEY;
			break;
		case 'D':
			type = DOOR;
			break;

	}
}

void Element::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 &s, glm::vec2 &posInSpritesheet, glm::vec2 &sizeInSpritesheet, int resistencia, vector<glm::vec2> &animations, char letter, int val)
{
	init(tileMapPos, shaderProgram, s, posInSpritesheet, sizeInSpritesheet, resistencia, animations, letter);
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
	return size;
}

int Element::collision()
{
	if (type != DOOR) {
		resistance--;
		if (resistance > 0) {
			int animId = sprite.getCurrentAnimation();
			sprite.changeAnimation(animId + 1);
		}
		else if (resistance == 0 && type != BRICK) {
			sprite.changeAnimation(0);
			if (!activateConsequence && (type == ALARM || type == KEY)) {
				activateConsequence = true;
			}
		}
	}

	return resistance;
}

bool Element::isFinished() {
	return sprite.isFinished();
}

bool Element::isActivated() {
	return activateConsequence;
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

void Element::activate() {
	int animId = sprite.getCurrentAnimation();
	sprite.changeAnimation(animId + 1);
}

void Element::executedConsequence()
{
	activateConsequence = false;
}



