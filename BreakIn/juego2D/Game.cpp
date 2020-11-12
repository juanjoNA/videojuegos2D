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
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();
	scene.init();
	setState(MENU, false);
}

void Game::setState(int state, bool fromPassword) {
	sceneAct = state;
	if (sceneAct == MENU) {
		GameSound->play2D("audio/breakout.mp3", true);
	}
	else if (sceneAct == GAME) {
		if (fromPassword) {
			fromPassword = false;
			scene.reinit(startingLevel);
		}
		GameSound->stopAllSounds();
	}
	else if (sceneAct == VERSUS) {
		sceneVersus.init();
		GameSound->stopAllSounds();
	}
	//Inicializar puntuación, vidas, sonido, ...
}

bool Game::update(int deltaTime)
{
	switch (sceneAct)
	{
	case MENU:
		menu.update();
		break;
	case GAME: 
		scene.update(deltaTime);
		break;
	case VERSUS:
		sceneVersus.update(deltaTime);
		break;
	};
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (sceneAct) {
	case MENU:
		menu.render();
		break;
	case GAME:
		scene.render();
		break;
	case VERSUS:
		sceneVersus.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	;
	if (key == 27) {
		bPlay = false;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (key == 49) {
		b1 = true;
	}
	else if (key == 50) {
		b2 = true;
	}
	else if (key == 51) {
		b3 = true;
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





