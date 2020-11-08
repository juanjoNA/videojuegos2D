#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "Game.h"
#include "Definitions.h"
#include <irrKlang.h>
using namespace irrklang;

#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps

#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 25

ISoundEngine *SceneSound = createIrrKlangDevice();

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
	map1 = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map3 = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	money = 0;
	lives = 4;
	points = 0;
	level = 3;
	switch (level) {
	case 1:
		map = map1;
		storeObjects();
		loadObjects("levels/OP_level01.txt");
		break;
	case 2:
		map = map2;
		storeObjects();
		loadObjects("levels/OP_level02.txt");
		break;
	case 3:
		map = map3;
		storeObjects();
		loadObjects("levels/OP_level03.txt");
		break;
	}
	subnivel = 1;

	tapadorTexture.loadFromFile("images/tapador.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pointsTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.0f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	levelTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.25f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	moneyTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	livesTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.75f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	roomTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.0f, 0.75f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	titlesTexture.loadFromFile("images/menuSpritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tapadorArriba = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);
	tapadorAbajo = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
	gameOverTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	gameOver.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);
	setGameTextPosition(subnivel);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT * 2));
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
	ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x)/2, player->getPosition().y-player->getSize().y));
	ball->setTileMap(map);

	police = new Police;
	police->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	police->setTileMap(map);

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
	player->update(deltaTime, subnivel);
	ball->update(deltaTime, objectsInGame, subnivel);
	police->update(deltaTime, player, subnivel);
	for (int i = 0; i < objectsInGame.size(); i++) {
		objectsInGame.at(i).update(deltaTime);
		if (objectsInGame.at(i).isActivated()) {
			if (objectsInGame.at(i).getType() == 2) { //ALARMA
				if (!police->isStarted()) police->setStart(true);
			}
			else {	//LLAVE

			}
		}
	}
	ballPos = ball->position();
	playerPos = player->getPosition();
	//cout << "points = " << points << endl;
	//cout << "money = " << money << endl << endl;
	cout << "posBall = (" << ballPos.x << " , " << ballPos.y << endl << endl;
	// Bajar nivel?
	switch (subnivel) {
	case 3:
		if (ballPos.y >= 464.f) {
			player->setPosition(glm::vec2(playerPos.x, playerPos.y + 448));
			subnivel = 2;
			setGameTextPosition(subnivel);
		}
		break;
	case 2:
		if (ballPos.y < 448.f) {
			player->setPosition(glm::vec2(playerPos.x, playerPos.y - 448));
			subnivel = 3;
			if(police->isStarted()) police->restart();
			setGameTextPosition(subnivel);
		}
		else if (ballPos.y >= 912.f) {
			player->setPosition(glm::vec2(playerPos.x, playerPos.y + 448));
			subnivel = 1;
			setGameTextPosition(subnivel);
		}
		break;
	case 1:
		if (ballPos.y < 896.f) {
			player->setPosition(glm::vec2(playerPos.x, playerPos.y - 448));
			subnivel = 2;
			setGameTextPosition(subnivel);
		}
		else if (ballPos.y >= 1328.f) {
			if (lives == 1) {
				--lives;
				SceneSound->play2D("audio/looseGame.wav");
			}
			else {
				pierdeVida();
			}
		}
		break;
	}

	if (police->catchPlayer()) {	//PIERDE VIDA Y SE REINICIA
		cout << "EL POLICIA HA PILLADO AL JUGADOR" << endl;
	}
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

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (lives > 0) {
		map->render();
		player->render();
		ball->render();
		tapadorArriba->render();
		tapadorAbajo->render();
		//police->render();
		for (int i = 0; i < objectsInGame.size(); i++) {
			if (objectsInGame.at(i).getType() != 0) {
				if (objectsInGame.at(i).isFinished()) {
					if (objectsInGame.at(i).getType() == 1) {
						money += objectsInGame.at(i).getValue();
					}
					else if (objectsInGame.at(i).getType() == 2) {
						points += 50;
					}

					//objectsInGame.at(i).free();
					objectsInGame.erase(objectsInGame.begin() + i);
				}
				else {
					objectsInGame.at(i).render();
				}
			}
		}
		text.render("MONEY", glm::vec2(510, 40), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(money), glm::vec2(510, 70), 25, glm::vec4(1, 1, 1, 1));
		text.render("POINTS", glm::vec2(510, 105), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(points), glm::vec2(510, 135), 25, glm::vec4(1, 1, 1, 1));
		text.render("LIVES", glm::vec2(510, 170), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(lives), glm::vec2(510, 200), 25, glm::vec4(1, 1, 1, 1));
		text.render("BANK", glm::vec2(510, 235), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(level), glm::vec2(510, 260), 25, glm::vec4(1, 1, 1, 1));
		text.render("ROOM", glm::vec2(510, 295), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(subnivel), glm::vec2(510, 320), 25, glm::vec4(1, 1, 1, 1));
		/*pointsTitle->render();
		moneyTitle->render();
		livesTitle->render();
		levelTitle->render();
		roomTitle->render();*/
	}
	else {
		reinit();
		gameOverTextureQuad->render(gameOver);
		Game::instance().setState(MENU);
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

void Scene::storeObjects()
{
	createBricks1();
	createBricks2();
	createBricks3();
	createMoney();
	createAlarm();
	createKey();
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
	int value;
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
		if (i == 0) value = 5;
		else if (i == 1) value = 10;
		else value = 50;
		coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'M', value);

		objVector.push_back(*coin);
		posIn.y += 0.3f;
	}

	Element *coin = new Element();
	vector<glm::vec2> animations;
	for (int j = 0; j < 4; j++) {
		animations.push_back(glm::vec2(0.05 * j, 0.f));
	}
	coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'M', 500);

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

bool Scene::loadObjects(const string &loadObjectsFile) {

	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int size, posObj;
	glm::ivec2 mapSize;

	fin.open(loadObjectsFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 14, "OBJECTPOSITION") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> size;

	int tileSize = map->getTileSize();
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin >> posObj;
			if (posObj >= 0) {
				Element e = objVector.at(posObj);
				e.setPosition(glm::vec2(i*tileSize, j*tileSize));
				objectsInGame.push_back(e);
				fin >> posObj;
				i++;
			}
		}
	}
	fin.close();

	return true;
}

void Scene::pierdeVida() {
	SceneSound->play2D("audio/gameOver.mp3");
	Sleep(2000);
	--lives;
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT * 2));
	ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
}

void Scene::setGameTextPosition(int subnivel) {
	if (subnivel == 3) {
		pointsTitle->setPosition(glm::vec2(430.0f, 16.0f));
		levelTitle->setPosition(glm::vec2(430.0f, 106.0f));
		moneyTitle->setPosition(glm::vec2(430.0f, 196.0f));
		livesTitle->setPosition(glm::vec2(430.0f, 286.0f));
		roomTitle->setPosition(glm::vec2(430.0f, 376.0f));
		tapadorArriba->setPosition(glm::vec2(SCREEN_X, 0.0f));
		tapadorAbajo->setPosition(glm::vec2(SCREEN_X, 464.0f));
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	}
	else if (subnivel == 2) {
		pointsTitle->setPosition(glm::vec2(430.0f, 16.0f + LEVEL_HEIGHT));
		levelTitle->setPosition(glm::vec2(430.0f, 106.0f + LEVEL_HEIGHT));
		moneyTitle->setPosition(glm::vec2(430.0f, 196.0f + LEVEL_HEIGHT));
		livesTitle->setPosition(glm::vec2(430.0f, 286.0f + LEVEL_HEIGHT));
		roomTitle->setPosition(glm::vec2(430.0f, 376.0f + LEVEL_HEIGHT));
		tapadorArriba->setPosition(glm::vec2(SCREEN_X, 448.0f));
		tapadorAbajo->setPosition(glm::vec2(SCREEN_X, 912.0f));
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(448 + SCREEN_HEIGHT - 1), 448.f);
	}
	else if (subnivel == 1) {
		pointsTitle->setPosition(glm::vec2(430.0f, 16.0f + LEVEL_HEIGHT * 2));
		levelTitle->setPosition(glm::vec2(430.0f, 106.0f + LEVEL_HEIGHT * 2));
		moneyTitle->setPosition(glm::vec2(430.0f, 196.0f + LEVEL_HEIGHT * 2));
		livesTitle->setPosition(glm::vec2(430.0f, 286.0f + LEVEL_HEIGHT * 2));
		roomTitle->setPosition(glm::vec2(430.0f, 376.0f + LEVEL_HEIGHT * 2));
		tapadorArriba->setPosition(glm::vec2(SCREEN_X, 896.0f));
		tapadorAbajo->setPosition(glm::vec2(SCREEN_X, 1376.0f));
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(896 + SCREEN_HEIGHT - 1), 896.f);
	}
}

void Scene::reinit()
{
	money = 0;
	lives = 4;
	points = 0;
	level = 3;
	switch (level) {
	case 1:
		map = map1;
		storeObjects();
		loadObjects("levels/OP_level01.txt");
		break;
	case 2:
		map = map2;
		storeObjects();
		loadObjects("levels/OP_level02.txt");
		break;
	case 3:
		map = map3;
		storeObjects();
		loadObjects("levels/OP_level03.txt");
		break;
	}
	subnivel = 1;

	setGameTextPosition(subnivel);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT * 2));
	player->setTileMap(map);

	ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
	ball->setTileMap(map);

	currentTime = 0.0f;
}
