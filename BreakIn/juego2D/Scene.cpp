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

	money = Game::instance().money;
	points = Game::instance().points;
	lives = Game::instance().lives;
	level = 1;
	subnivel = 1;

	Game::instance().godMode = false;

	map1 = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map3 = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

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

	titlesTexture.loadFromFile("images/menuSpritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tapadorTexture.loadFromFile("images/tapador.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOverTexture.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);

	pointsTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.0f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	levelTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.25f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	moneyTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	livesTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.75f, 0.5f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	roomTitle = Sprite::createSprite(glm::ivec2(80, 40), glm::vec2(0.0f, 0.75f), glm::vec2(0.25f, 0.25f), &titlesTexture, &texProgram, false);
	tapadorArriba = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);
	tapadorAbajo = Sprite::createSprite(glm::ivec2(384.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &tapadorTexture, &texProgram, false);

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
	bool open = false;
	for (int i = objectsInGame.size()-1; i >= 0 ; i--) {
		objectsInGame.at(i).update(deltaTime);
		if (objectsInGame.at(i).isActivated()) {
			if (objectsInGame.at(i).getType() == 2) { //ALARMA
				if (!police->isStarted()) police->setStart(true);
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
	}
	ballPos = ball->position();
	playerPos = player->getPosition();
	//cout << "points = " << points << endl;
	//cout << "money = " << money << endl << endl;
	//cout << "posBall = (" << ballPos.x << " , " << ballPos.y << endl << endl;
	// Bajar nivel?
	if (Game::instance().bF1) {
		Game::instance().bF1 = false;
		if (level != 1) {
			Game::instance().money += money;
			Game::instance().lives = lives;
			Game::instance().points += points;
			Game::instance().sceneAnt = TELE_1;
			Game::instance().setState(ANIMATION);
		}
	}
	else if (Game::instance().bF2) {
		Game::instance().bF2 = false;
		if (level != 2) {
			Game::instance().money += money;
			Game::instance().lives = lives;
			Game::instance().points += points;
			Game::instance().sceneAnt = TELE_2;
			Game::instance().setState(ANIMATION);
		}
	}
	else if (Game::instance().bF3) {
		Game::instance().bF3 = false;
		if (level != 3) {
			Game::instance().money += money;
			Game::instance().lives = lives;
			Game::instance().points += points;
			Game::instance().sceneAnt = TELE_3;
			Game::instance().setState(ANIMATION);
		}
	}
	else {
		switch (subnivel) {
		case 3:
			if (Game::instance().bF4) {
				Game::instance().bF4 = false;
				subeNivel();
			}
			else if (Game::instance().bF5) {
				Game::instance().bF5 = false;
				player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT));
				player->setTileMap(map);

				ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
				ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
				ball->setTileMap(map);
				subnivel = 2;
				setGameTextPosition(subnivel);
			}
			else if (ballPos.y >= 464.f) {
				player->setPosition(glm::vec2(playerPos.x, playerPos.y + 448));
				subnivel = 2;
				setGameTextPosition(subnivel);
			}
			break;

		case 2:
			if (Game::instance().bF4) {
				Game::instance().bF4 = false;
				player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
				player->setTileMap(map);

				ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
				ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
				ball->setTileMap(map);
				subnivel = 3;
				setGameTextPosition(subnivel);
			}
			else if (Game::instance().bF5) {
				Game::instance().bF5 = false;
				player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT * 2));
				player->setTileMap(map);

				ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
				ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
				ball->setTileMap(map);
				subnivel = 1;
				setGameTextPosition(subnivel);
			}
			else if (ballPos.y < 448.f) {
				player->setPosition(glm::vec2(playerPos.x, playerPos.y - 448));
				subnivel = 3;
				setGameTextPosition(subnivel);
			}
			else if (ballPos.y >= 912.f) {
				player->setPosition(glm::vec2(playerPos.x, playerPos.y + 448));
				subnivel = 1;
				setGameTextPosition(subnivel);
			}
			break;

		case 1:
			if (Game::instance().bF4) {
				Game::instance().bF4 = false;
				player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT));
				player->setTileMap(map);

				ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
				ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
				ball->setTileMap(map);
				subnivel = 2;
				setGameTextPosition(subnivel);
			}
			else if (Game::instance().bF5) {
				Game::instance().bF5 = false;
				if (lives == 1) {
					--lives;
					SceneSound->play2D("audio/looseGame.wav");
					Game::instance().sceneAnt = GAME_OVER;
					Game::instance().setState(ANIMATION);
				}
				else {
					pierdeVida();
				}
			}
			else if (ballPos.y < 896.f) {
				player->setPosition(glm::vec2(playerPos.x, playerPos.y - 448));
				subnivel = 2;
				setGameTextPosition(subnivel);
			}
			else if (ballPos.y >= 1328.f) {
				if (lives == 1) {
					--lives;
					SceneSound->play2D("audio/looseGame.wav");
					Game::instance().sceneAnt = GAME_OVER;
					Game::instance().setState(ANIMATION);
				}
				else {
					pierdeVida();
				}
			}
			break;
		}
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

		map->render();
		for (int i = objectsInGame.size()-1; i >= 0 ; i--) {
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
			else if (objectsInGame.at(i).getResistance() == 0) {
					points += 50;
					//objectsInGame.at(i).free();
					objectsInGame.erase(objectsInGame.begin() + i);
			}
			else {
					objectsInGame.at(i).render();
			}
		}

		player->render();
		ball->render();
		tapadorArriba->render();
		tapadorAbajo->render();
		police->render();

		pointsTitle->render();
		moneyTitle->render();
		livesTitle->render();
		levelTitle->render();
		roomTitle->render();

		text.render(to_string(money), glm::vec2(430, 85), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(points), glm::vec2(430, 180), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(lives), glm::vec2(430, 275), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(level), glm::vec2(430, 370), 25, glm::vec4(1, 1, 1, 1));
		text.render(to_string(subnivel), glm::vec2(430, 465), 25, glm::vec4(1, 1, 1, 1));
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
	createDoor();
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
	vector<glm::vec2> animations(4);
	animations[0] = glm::vec2(0.f, 0.f);
	animations[1] = glm::vec2(0.05f, 0.f);
	animations[2] = glm::vec2(0.1f, 0.f);
	animations[3] = glm::vec2(0.15f, 0.f);

	key->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'K');
	objVector.push_back(*key);
}

void Scene::createAlarm()
{
	glm::vec2 posIn = glm::vec2(0.6f, 0.6f);
	glm::vec2 sizeIn(0.05f, 0.1f);
	alarm = new Element();
	vector<glm::vec2> animations(5);
	animations[0] = glm::vec2(0.f, 0.f);
	animations[1] = glm::vec2(0.05f, 0.f);
	animations[2] = glm::vec2(0.1f, 0.f);
	animations[3] = glm::vec2(0.15f, 0.f);
	animations[4] = glm::vec2(0.f, 0.1f);
	alarm->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(32, 32), posIn, sizeIn, 1, animations, 'A');
	objVector.push_back(*alarm);
}

void Scene::createDoor()
{
	glm::vec2 posIn = glm::vec2(0.7f, 0.7f);
	glm::vec2 sizeIn(0.1f, 0.1f);
	door = new Element();
	vector<glm::vec2> animations(3);
	animations[0] = glm::vec2(0.f, 0.f);
	animations[1] = glm::vec2(0.f, 0.1f);
	animations[2] = glm::vec2(0.f, 0.2f);
	door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(160, 16), posIn, sizeIn, 1, animations, 'D');
	objVector.push_back(*door);
}


bool Scene::loadObjects(const string &loadObjectsFile) {

	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int size, posObj;
	glm::ivec2 mapSize;
	objectsInGame.clear();

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

void Scene::subeNivel() {
	Game::instance().money = money;
	Game::instance().lives = lives + 3;
	Game::instance().points = points;
	Game::instance().sceneAnt = level;
	Game::instance().setState(ANIMATION);
}

void Scene::setGameTextPosition(int subnivel) {
	float despl;
	if (subnivel == 1) {
		despl = LEVEL_HEIGHT * 2;
	}
	else if (subnivel == 2) {
		despl = LEVEL_HEIGHT;
	}
	else if (subnivel == 3) {
		despl = 0;
	}

	moneyTitle->setPosition(glm::vec2(430.0f, 16.0f + despl));
	pointsTitle->setPosition(glm::vec2(430.0f, 106.0f + despl));
	livesTitle->setPosition(glm::vec2(430.0f, 196.0f + despl));
	levelTitle->setPosition(glm::vec2(430.0f, 286.0f + despl));
	roomTitle->setPosition(glm::vec2(430.0f, 376.0f + despl));
	tapadorArriba->setPosition(glm::vec2(SCREEN_X, 0.0f + despl));
	tapadorAbajo->setPosition(glm::vec2(SCREEN_X, 464.0f + despl));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(despl + SCREEN_HEIGHT - 1), despl);
}

void Scene::reinit(int reinitLevel)
{
	money = Game::instance().money;
	points = Game::instance().points;
	lives = Game::instance().lives;
	level = reinitLevel;
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

	Game::instance().godMode = false;

	setGameTextPosition(subnivel);

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + LEVEL_HEIGHT * 2));
	player->setTileMap(map);

	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player);
	ball->setPosition(glm::vec2(player->getPosition().x + (player->getSize().x) / 2, player->getPosition().y - player->getSize().y));
	ball->setTileMap(map);

	police->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	police->setTileMap(map);

	currentTime = 0.0f;
}
