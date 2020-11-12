#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Definitions.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *GameSound = createIrrKlangDevice();


void Game::init()
{
	bPlay = true;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	setState(ANIMATION);
	sceneAnt = -1;
}

void Game::initGamePropierties() {
	money = 0;
	lives = 4;
	points = 0;
}

void Game::setState(int state) {
	sceneAct = state;
	if (sceneAct == MENU) {
		initGamePropierties();
		if (sceneAnt == -1) {
			menu.init();
			scene.init();
			Game::instance().sceneAnt = ANIMATION;
		}
		else if (sceneAnt == VERSUS) {

		}
		else {
			scene.reinit(1);
			Game::instance().sceneAnt = ANIMATION;
		}
		GameSound->play2D("audio/breakout.mp3", true);
	}
	else if (sceneAct == VERSUS) {
		sceneVersus.init();
		GameSound->stopAllSounds();
	}
	else if (sceneAct == LEVEL_1) {
		if (sceneAnt != MENU) {
			scene.reinit(1);
		}
		GameSound->stopAllSounds();
	}
	else if (sceneAct == LEVEL_2) {
		scene.reinit(2);
		GameSound->stopAllSounds();
	}
	else if (sceneAct == LEVEL_3) {
		scene.reinit(3);
		GameSound->stopAllSounds();
	}
	else if (sceneAct == ANIMATION) {
		animation.init();
	}
	//Inicializar puntuación, vidas, sonido, ...
}

bool Game::update(int deltaTime)
{
	if (sceneAct == MENU) menu.update();
	else if (sceneAct == VERSUS) sceneVersus.update(deltaTime);
	else if (sceneAct == LEVEL_1 || sceneAct == LEVEL_2 || sceneAct == LEVEL_3) scene.update(deltaTime);
	else if (sceneAct == ANIMATION) animation.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (sceneAct == MENU) menu.render();
	else if (sceneAct == VERSUS) sceneVersus.render();
	else if (sceneAct == LEVEL_1 || sceneAct == LEVEL_2 || sceneAct == LEVEL_3) scene.render();
	else if (sceneAct == ANIMATION) animation.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) {
		bPlay = false;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (key == 8) {
		borra = true;
	}
	else if (key == 13) {
		enterPressed = true;
	}
	else if (key == 32) {
		spacePressed = true;
	}
	else if (key >= 65 && key <= 90) {
		letra = char(key);
		nuevaLetra = true;
	}
	else if (key >= 97 && key <= 122) {
		letra = char(key - 32);
		nuevaLetra = true;
	}
	
	if (key == 103) {
		if (!godMode) godMode = true;
		else godMode = false;
	}
	else if (key == 98) {
		if (!destroyBlocks) destroyBlocks = true;
	}
	else if (key == 99) {
		if (!pickMoney) pickMoney = true;
	}
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	if (key == 1) {
		bF1 = true;
	}
	else if (key == 2) {
		bF2 = true;
	}
	else if (key == 3) {
		bF3 = true;
	}
	else if (key == 4) {
		bF4 = true;
	}
	else if (key == 5) {
		bF5 = true;
	}
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





