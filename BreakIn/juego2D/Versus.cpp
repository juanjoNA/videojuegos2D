#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Versus.h"
#include "Game.h"
#include "Definitions.h"
#include <irrKlang.h>
using namespace irrklang;

#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps

#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER1_X_TILES 3
#define INIT_PLAYER1_Y_TILES 11

#define INIT_PLAYER2_X_TILES 45
#define INIT_PLAYER2_Y_TILES 11

ISoundEngine *VersusSound = createIrrKlangDevice();

Versus::Versus()
{
	res1 = 0;
	res2 = 0;
}

Versus::~Versus()
{
	if (map != NULL)
		delete map;
	if (player1 != NULL)
		delete player1;
	if (player2 != NULL)
		delete player2;
	if (ball != NULL)
		delete ball;
}

void Versus::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/versusMap.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
	
	player1 = new Player();
	player1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 64));
	player1->setPosition(glm::vec2(INIT_PLAYER1_X_TILES * map->getTileSize(), INIT_PLAYER1_Y_TILES * map->getTileSize()));
	player1->setTileMap(map);
	player1->setSpeed(8);

	player2 = new Player();
	player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(16, 64));
	player2->setPosition(glm::vec2(INIT_PLAYER2_X_TILES * map->getTileSize(), INIT_PLAYER2_Y_TILES * map->getTileSize()));
	player2->setTileMap(map);
	player2->setSpeed(8);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2((map->getMapSize().x*map->getTileSize()/2)-8, (map->getMapSize().y*map->getTileSize() / 2)+24));
	ball->setTileMap(map);
	ball->setVelocity(glm::vec2(8, 5));

	projection = glm::ortho(0.f, float(800 - 1), float(560 - 1), 0.f);
	currentTime = 0.0f;
}

void Versus::update(int deltaTime)
{
	currentTime += deltaTime;
	player1->update(deltaTime, -1, 0);
	player2->update(deltaTime, -2, 1);
	vector<class Element> e;
	glm::vec2 posBall = ball->position();
	if (posBall.x < map->getMapSize().x*map->getTileSize()/2) {
		ball->update(deltaTime, e, player1);
		if (posBall.x <= 0) goal(2);
	}
	else {
		ball->update(deltaTime, e, player2);
		if (posBall.x > (map->getMapSize().x*map->getTileSize())) goal(1);
	}
	
	
	/*for (int i = objectsInGame.size() - 1; i >= 0; i--) {
		objectsInGame.at(i).update(deltaTime);
		if (objectsInGame.at(i).isActivated()) {
			if (objectsInGame.at(i).getType() == 2) { //ALARMA
				if (!police->isStarted()) police->setStart(true);
				else if (police->catchPlayer()) {
					pierdeVida();
					police->setColision(false);
					objectsInGame.at(i).executedConsequence();
				}
			}
			else {	//LLAVE
				open = true;
				objectsInGame.at(i).executedConsequence();
			}
		}
		if (objectsInGame.at(i).getType() == 4 && open) {
			open = false;
			objectsInGame.at(i).activate();
		}
	}*/

}

void Versus::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	player1->render();
	player2->render();
	ball->render();
		
	
	/*else {
		reinit(1);
		gameOverTextureQuad->render(gameOver);
		Sleep(2000);
		Game::instance().setState(MENU, false);
	}*/
}

void Versus::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Versus::winner(int win) 
{

}

void Versus::goal(int jugador) {
	if (jugador == 1) {
		res1++;
		if (res1 == 7) winner(jugador);
		reinit(2);
	}
	else {
		res2++;
		if (res2 == 7) winner(jugador);
		reinit(1);
	}
}

void Versus::reinit(int playerPos)
{
	player1->setPosition(glm::vec2(INIT_PLAYER1_X_TILES * map->getTileSize(), INIT_PLAYER1_Y_TILES * map->getTileSize()));
	player1->restart();
	player2->setPosition(glm::vec2(INIT_PLAYER2_X_TILES * map->getTileSize(), INIT_PLAYER2_Y_TILES * map->getTileSize()));
	player2->restart();

	if (playerPos == 1) {
		ball->setPosition(glm::vec2(player1->getPosition().x+player1->getSize().x, player1->getPosition().y + (player1->getSize().y/2)));
		//ball->setVelocity(glm::vec2(5, 5));
	}
	else {
		ball->setPosition(glm::vec2(player2->getPosition().x - player2->getSize().x, player2->getPosition().y + (player2->getSize().y / 2)));
		//ball->setVelocity(glm::vec2(-5, 5));
	}
	ball->finish();
}
