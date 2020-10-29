#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Definitions.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	setState(MENU);
}

void Game::setState(int state) {
	sceneAct = state;
	if (sceneAct == MENU) {
		menu.init();
		SoundEngine->play2D("audio/breakout.mp3", true);
	}
	else if (sceneAct == GAME) {
		scene.init();
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/level1.mp3", true);
	}
	//Inicializar puntuación, vidas, sonido, ...
}

bool Game::update(int deltaTime)
{
	if (sceneAct == MENU) menu.update();
	else if (sceneAct == GAME) scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (sceneAct == MENU) menu.render();
	else if (sceneAct == GAME) scene.render();
}

void Game::keyPressed(int key)
{;
if (key == 27) {
	bPlay = false;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
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





