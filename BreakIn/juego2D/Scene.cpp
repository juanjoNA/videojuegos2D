#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "Game.h"

#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps

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
	subnivel = 0;

	tapadorTexture.loadFromFile("images/tapador.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tapadorArriba = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);
	tapadorArriba->setPosition(glm::vec2(SCREEN_X, 0.0f));
	tapadorAbajo = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);
	tapadorAbajo->setPosition(glm::vec2(SCREEN_X, 464.0f));

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 10, INIT_PLAYER_Y_TILES * map->getTileSize() - 100));
	ball->setTileMap(map);

	police = new Police;
	police->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	police->setPosition(glm::vec2(20 * map->getTileSize(), 20 * map->getTileSize() - 100));
	police->setTileMap(map);

	//createObjects();
	/*glm::vec2 posicion = glm::vec2(40, 32);
	for (int i = 0; i < bricks.size(); i++) {
		bricks.at(i).setPosition(posicion);
		posicion.x += 32.f;
	}
	glm::vec2 posicion2 = glm::vec2(40, 256);
	for (int i = 0; i < money.size(); i++) {
		money.at(i).setPosition(posicion2);
		posicion2.x += 64.f;
	}*/

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	// Select which font you want to use
	 if (!text.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	police->update(deltaTime, player);
	ball->update(deltaTime, money, bricks, subnivel);
	ballPos = ball->position();
	// Bajar nivel?
	if (ballPos.x > 144 && ballPos.x < 272 && ballPos.y >= 464 && yAnterior < ballPos.y && subnivel != 2) {
		switch (subnivel) {
		case 0:
			map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y - 464), texProgram);
			subnivel = 1;
			ball->setPosition(glm::vec2(ballPos.x, 16.0f));
			break;
		case 1:
			map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y - 928), texProgram);
			subnivel = 2;
			ball->setPosition(glm::vec2(ballPos.x, 16.0f));
			break;
		}
		yAnterior = 0.0f;
		/*duration = 2.0;
		timeFin = currentTime + duration;
		nSteps = duration / TIME_PER_FRAME;
		translationDone = 0.0;
		translation = -464.0 * nSteps;
		isTransitioning = true;
	}

	if (currentTime <= timeFin) {
		translationDone += translation;
	}
	else {
		isTransitioning = false;
	}*/
	}
	// Subir nivel?
	else if (ballPos.x > 144 && ballPos.x < 272 && ballPos.y <= 16 && yAnterior > ballPos.y && subnivel != 0) {
		switch (subnivel) {
		case 1:
			map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			subnivel = 0;
			ball->setPosition(glm::vec2(ballPos.x, 464.0f));
			break;
		case 2:
			map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y - 464), texProgram);
			subnivel = 1;
			ball->setPosition(glm::vec2(ballPos.x, 464.0f));
			break;
		}
		yAnterior = 480.0f;
	}
	else {
		yAnterior = ballPos.y;
	}

	/*for (int i = 0; i < bricks.size(); i++) {
		bricks.at(i).update(deltaTime);		
	}
	for (int i = 0; i < money.size(); i++) {
		money.at(i).update(deltaTime);
	}*/
	//key->update(deltaTime);
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
	tapadorAbajo->render();
	tapadorArriba->render();

	/*for (int i = 0; i < bricks.size(); i++) {
		if (bricks.at(i).getResistance() == 0) {
			bricks.erase(bricks.begin() + i);
		}
		else {
			bricks.at(i).render();
		}
	}
	for (int i = 0; i < money.size(); i++) {
		if (money.at(i).isFinished()) {
			money.erase(money.begin() + i);
		}
		else {
			money.at(i).render();
		}
	}*/
	//key->render();
	text.render("Videogames!!!", glm::vec2(10, SCREEN_HEIGHT - 20), 32, glm::vec4(1, 1, 1, 1));
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
	createBricks1();
	createBricks2();
	createBricks3();
	createMoney();
	createKey();
	createAlarm();
}

void Scene::createBricks1()
{
	glm::vec2 posIn = glm::vec2(0, 0);
	glm::vec2 sizeIn(0.2f, 0.2f);
	vector<glm::vec2> animations;
	for (int i = 0; i < 3; i++) {
		Element *brick = new Element();
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 1, animations, 'B');
		objVector.push_back(*brick);
		posIn.y += 0.2f;
	}

}

void Scene::createBricks2()
{
	glm::vec2 posIn = glm::vec2(0.f, 0.6f);
	glm::vec2 sizeIn(0.2f, 0.2f);
	for (int i = 0; i < 3; i++) {
		Element *brick = new Element();
		vector<glm::vec2> animations;
		for (int j = 0; j < 2; j++) {
			animations.push_back(glm::vec2(0.2f*j, 0.f));
		}
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 2, animations, 'B');

		objVector.push_back(*brick);
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
		Element *brick = new Element();
		vector<glm::vec2> animations;
		for (int j = 0; j < 3; j++) {
			animations.push_back(glm::vec2(0.2f*j, 0.f));
		}
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 16), posIn, sizeIn, 3, animations, 'B');

		objVector.push_back(*brick);
		posIn.y += 0.2f;
	}
}

void Scene::createMoney()
{
	glm::vec2 posIn = glm::vec2(0.8f, 0.f);
	glm::vec2 sizeIn(0.05f, 0.1f);
	for (int i = 0; i < 3; i++) {
		Element *coin = new Element();
		vector<glm::vec2> animations;
		float x = 0.f;
		float y = 0.f;
		for (int j = 0; j < 9; j++) {
			x = 0.05 * (j % 4);
			y = 0.1 * (j / 4);
			animations.push_back(glm::vec2(x, y));
		}
		coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'M');

		objVector.push_back(*coin);
		posIn.y += 0.3f;
	}

	Element *coin = new Element();
	vector<glm::vec2> animations;
	for (int j = 0; j < 4; j++) {
		animations.push_back(glm::vec2(0.05 * j, 0.f));
	}
	coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'M');

	objVector.push_back(*coin);
}

void Scene::createKey()
{
	glm::vec2 posIn = glm::vec2(0.4f, 0.6f);
	glm::vec2 sizeIn(0.05f, 0.1f);
	key = new Element();
	vector<glm::vec2> animations;
	for (int j = 0; j < 4; j++) {
		animations.push_back(glm::vec2(0.05 * j, 0.f));
	}
	key->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'K');
	objVector.push_back(*key);
}

void Scene::createAlarm()
{
	glm::vec2 posIn = glm::vec2(0.6f, 0.6f);
	glm::vec2 sizeIn(0.05f, 0.1f);
	alarm = new Element();
	vector<glm::vec2> animations;
	for (int j = 0; j < 4; j++) {
		animations.push_back(glm::vec2(0.05 * j, 0.f));
	}
	alarm->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'A');
	objVector.push_back(*alarm);
}


