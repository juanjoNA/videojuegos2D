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
	if (map != NULL)
		delete map;
	if (player != NULL)
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
	for (int i = 0; i < objects.size(); i++) {
		objects.at(i).setPosition(posicion);
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
	for (int i = 0; i < objects.size(); i++) {
		objects.at(i).update(deltaTime);
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
	for (int i = 0; i < objects.size(); i++) {
		objects.at(i).render();
	}
}

void Scene::initShaders()
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

void Scene::createObjects()
{
	createBricks3();
}

void Scene::createBricks1()
{
	glm::vec2 posIn = glm::vec2(0, 0);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 3; i++) {
		Component *brick = new Component();
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 1, NULL);
		objects.push_back(*brick);
		posIn.y += 0.2f;
	}

}

void Scene::createBricks2()
{
	for (int i = 0; i <= 3; i++) {

	}
}

void Scene::createBricks3()
{
	glm::vec2 posIn = glm::vec2(0.2f, 0.4f);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 2; i++) {
		Component *brick = new Component();
		glm::vec2 keyframes[3];
		for (int j = 0; j < 3; j++) {
			keyframes[j] = glm::vec2(posIn.x + 0.2f*j, posIn.y);
		}
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 3, keyframes);

		objects.push_back(*brick);
		posIn.y += 0.2f;
	}
}

void Scene::createMoney()
{
	for (int i = 0; i <= 4; i++) {

	}
}

void Scene::createKey()
{
}

void Scene::createAlarm()
{
}