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

	controlsTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	controlsTexture.loadFromFile("images/gameOverBackground.jpeg", TEXTURE_PIXEL_FORMAT_RGB);

	initialTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	initialTexture.loadFromFile("images/loadingPage.png", TEXTURE_PIXEL_FORMAT_RGBA);

	gameOverTexture.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOver = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), &gameOverTexture, &texProgram, false);
	gameOver->setPosition(glm::vec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4));

	thiefTexture.loadFromFile("images/thiefSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	thief = Sprite::createSprite(glm::ivec2(320, 64), glm::vec2(0.f, 0.6f), glm::vec2(0.2f, 0.25f), &thiefTexture, &texProgram, false);
	thief->setPosition(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	thief->setNumberAnimations(3);
		//Animacion 0
		thief->setAnimationSpeed(0, 20);
		thief->addKeyframe(0, glm::vec2(0.f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.50f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.75f, 0.f));
		thief->addKeyframe(0, glm::vec2(-0.75f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.f, 0.2f));
		thief->addKeyframe(0, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.50f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.75f, 0.f));
		thief->addKeyframe(0, glm::vec2(-0.75f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.f, -0.8f));
		thief->addKeyframe(0, glm::vec2(0.f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.5f, 0.f));
		thief->addKeyframe(0, glm::vec2(0.75f, 0.f));

		//Animacion 1
		thief->setAnimationSpeed(1, 20);
		//miro izquierda
		thief->addKeyframe(1, glm::vec2(0.f, 0.2f));
		thief->addKeyframe(1, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.50f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.75f, 0.f));
		thief->addKeyframe(1, glm::vec2(-0.75f, 0.f));
		//miro derecha
		thief->addKeyframe(1, glm::vec2(0.f, -0.2f));
		thief->addKeyframe(1, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.50f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.75f, 0.f));
		thief->addKeyframe(1, glm::vec2(-0.75f, 0.f));
		//Troto
		thief->addKeyframe(1, glm::vec2(0.f, -0.2f));
		thief->addKeyframe(1, glm::vec2(0.25f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.5f, 0.f));
		thief->addKeyframe(1, glm::vec2(0.75f, 0.f));

		//Animacion 2
		thief->addKeyframe(2, glm::vec2(0.f, -0.4f));
		thief->addKeyframe(2, glm::vec2(0.25f, 0.f));
	thief->changeAnimation(0);

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
		thief->update(deltaTime);
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
	
	if (Game::instance().sceneAnt == -1) {
		initialTextureQuad->render(initialTexture);
	}
	else if (Game::instance().sceneAnt == GAME_OVER) {
		controlsTextureQuad->render(controlsTexture);
		gameOver->render();
	}
	else if (Game::instance().sceneAnt == LEVEL_1) {
		thief->render();
		controlsTextureQuad->render(controlsTexture);
		text.render("Transicion del level1 al level2", glm::vec2(230, 80), 25, glm::vec4(1, 1, 1, 1));
	}
	else if (Game::instance().sceneAnt == LEVEL_2) {
		controlsTextureQuad->render(controlsTexture);
		text.render("Transicion del level2 al level3", glm::vec2(230, 80), 25, glm::vec4(1, 1, 1, 1));
	}
	else if (Game::instance().sceneAnt == LEVEL_3) {
		controlsTextureQuad->render(controlsTexture);
		text.render("Animación victoria", glm::vec2(230, 80), 25, glm::vec4(1, 1, 1, 1));
	}
	else if (Game::instance().sceneAnt == TELE_1) {
		controlsTextureQuad->render(controlsTexture);
		text.render("Teletransporting to Level 1", glm::vec2(65, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (Game::instance().sceneAnt == TELE_2) {
		controlsTextureQuad->render(controlsTexture);
		text.render("Teletransporting to Level 2", glm::vec2(65, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (Game::instance().sceneAnt == TELE_3) {
		controlsTextureQuad->render(controlsTexture);
		text.render("Teletransporting to Level 3", glm::vec2(65, SCREEN_HEIGHT / 2 + 35 / 2), 35, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
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