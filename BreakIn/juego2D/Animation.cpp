#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "Game.h"
#include "Definitions.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Animation::Animation()
{
}

Animation::~Animation()
{
}


void Animation::init()
{
	initShaders();

	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };

	TextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);

	controlsTexture.loadFromFile("images/gameOverBackground.jpeg", TEXTURE_PIXEL_FORMAT_RGB);

	initialTexture.loadFromFile("images/loadingPage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	transition1Texture.loadFromFile("images/transicion_level1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	transition2Texture.loadFromFile("images/transicion_level2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTexture.loadFromFile("images/transicion_win.png", TEXTURE_PIXEL_FORMAT_RGBA);
	controlsTexture.loadFromFile("images/gameOverBackground.jpeg", TEXTURE_PIXEL_FORMAT_RGB);

	gameOverTexture.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOver = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &gameOverTexture, &texProgram, false);
	gameOver->setPosition(glm::vec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	currentTime = 0.0f;

	// Select which font you want to use
	if (!text.init("fonts/LemonMilk.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Animation::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().sceneAnt == -1 || Game::instance().sceneAnt == GAME_OVER || Game::instance().sceneAnt == LEVEL_3) {
		Game::instance().setState(MENU);
	}
	else if (Game::instance().sceneAnt == TELE_1) {
		Game::instance().setState(LEVEL_1);
	}
	else if (Game::instance().sceneAnt == LEVEL_1 || Game::instance().sceneAnt == TELE_2) {

		Game::instance().setState(LEVEL_2);
	}
	else if (Game::instance().sceneAnt == LEVEL_2 || Game::instance().sceneAnt == TELE_3) {
		Game::instance().setState(LEVEL_3);
	}
}

void Animation::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	int m = Game::instance().money;

	if (Game::instance().sceneAnt == -1) {
		TextureQuad->render(initialTexture);
	}
	else if (Game::instance().sceneAnt == GAME_OVER) {
		TextureQuad->render(controlsTexture);
		gameOver->render();
	}
	else if (Game::instance().sceneAnt == LEVEL_1) {
		TextureQuad->render(transition1Texture);
		text.render("Road to level 2", glm::vec2((SCREEN_WIDTH / 2) - 140, 50), 34, glm::vec4(1, 1, 0, 1));
		string dinero = to_string(m) + " EUR";
		text.render(dinero, glm::vec2((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 15), 40, glm::vec4(1, 1, 0, 1));
	}
	else if (Game::instance().sceneAnt == LEVEL_2) {
		TextureQuad->render(transition2Texture);
		text.render("Road to level 3", glm::vec2((SCREEN_WIDTH / 2) - 150, 50), 34, glm::vec4(1, 1, 0, 1));
		string dinero = to_string(m) + " EUR";
		text.render(dinero, glm::vec2((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 5), 40, glm::vec4(1, 1, 0, 1));
	}
	else if (Game::instance().sceneAnt == LEVEL_3) {
		TextureQuad->render(winTexture);
		text.render("YOU WIN!!", glm::vec2((SCREEN_WIDTH / 2) - 125, 100), 45, glm::vec4(1, 1, 0, 1));
		string dinero = to_string(m) + " EUR";
		text.render(dinero, glm::vec2((SCREEN_WIDTH / 2) - 130, 170), 45, glm::vec4(1, 1, 0, 1));
	}
	else if (Game::instance().sceneAnt == TELE_1) {
		TextureQuad->render(controlsTexture);
		text.render("Teleporting to Level 1", glm::vec2((SCREEN_WIDTH / 2)-200, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (Game::instance().sceneAnt == TELE_2) {
		TextureQuad->render(controlsTexture);
		text.render("Teleporting to Level 2", glm::vec2((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (Game::instance().sceneAnt == TELE_3) {
		TextureQuad->render(controlsTexture);
		text.render("Teleporting to Level 3", glm::vec2((SCREEN_WIDTH / 2) - 200, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void Animation::initShaders()
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