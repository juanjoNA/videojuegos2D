#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 15


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() - 100));
	ball->setTileMap(map);

	createObjects();
	glm::vec2 posicion = glm::vec2(40, 32);
	for (int i = 0; i < bricks.size(); i++) {
		bricks.at(i).setPosition(posicion);
		posicion.x += 32.f;
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);
	for (int i = 0; i < bricks.size(); i++) {
		bricks.at(i).update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	ball->render();
	for (int i = 0; i < bricks.size(); i++) {
		bricks.at(i).render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::createObjects()
{
	createBricks1();
	createBricks2();
	createBricks3();
	createMoney();
}

void Scene::createBricks1()
{
	glm::vec2 posIn = glm::vec2(0,0);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 3; i++) {
		Component *brick = new Component();
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32,16),posIn, sizeIn, 1, NULL);
		bricks.push_back(*brick);
		posIn.y += 0.2f;
	}

}

void Scene::createBricks2()
{
	glm::vec2 posIn = glm::vec2(0.f, 0.6f);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 3; i++) {
		Component *brick = new Component();
		glm::vec2 animations[2];
		for (int j = 0; j < 2; j++) {
			animations[j] = glm::vec2(0.2f*j, 0.f);
		}
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 2, animations);

		bricks.push_back(*brick);
		posIn.y += 0.2f;
		if (posIn.y >= 1.f) {
			posIn = glm::vec2(0.2f, 0.f);
		}
	}
}

void Scene::createBricks3()
{
	glm::vec2 posIn = glm::vec2(0.2f, 0.2f);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 2; i++) {
		Component *brick = new Component();
		glm::vec2 animations[3];
		for (int j = 0; j < 3; j++) {
			animations[j] = glm::vec2(0.2f*j, 0.f);
		}
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 3, animations);
		
		bricks.push_back(*brick);
		posIn.y += 0.2f;
	}
}

void Scene::createMoney()
{
	glm::vec2 posIn = glm::vec2(0.8f, 0.f);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i <= 3; i++) {
		Component *coin = new Component();
		glm::vec2 animations[9];
		float x = 0.f;
		float y = 0.f;
		for (int j = 0; j < 9; j++) {
			x = 0.05 * (j%4);
			y = 0.1 * (j / 4);
			animations[j] = glm::vec2(x, y);
		}
		coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 1, animations);

		money.push_back(*coin);
		posIn.y += 0.3f;
	}

	Component *coin = new Component();
	glm::vec2 animations[4];
	for (int j = 0; j < 4; j++) {
		animations[j] = glm::vec2(0.05 * j, 0.f);
	}
	coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 1, animations);

	money.push_back(*coin);
}

void Scene::createKey()
{
}

void Scene::createAlarm()
{
}



