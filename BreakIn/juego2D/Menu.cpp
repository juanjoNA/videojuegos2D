#include "Menu.h"
#include "Definitions.h"
#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *MenuSound = createIrrKlangDevice();

#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

int firstButtonY = SCREEN_HEIGHT / 2 - 64;

Menu::Menu()
{
}

void Menu::init() {
	initShaders();

	index = 0; //Option by default
	numOptions = 5;
	bUpPressed = false;
	bDownPressed = false;
	bEnterPressed = false;

	bCredits = false;
	bPassword = false;
	bControls = false;

	passwordAtempt = "";

	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };

	mainTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	mainTexture.loadFromFile("images/fondoBreakIn.png", TEXTURE_PIXEL_FORMAT_RGB);

	controlsTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	controlsTexture.loadFromFile("images/controls.png", TEXTURE_PIXEL_FORMAT_RGB);

	creditsTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	creditsTexture.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);

	passwordTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	passwordTexture.loadFromFile("images/password.png", TEXTURE_PIXEL_FORMAT_RGB);

	menuTexture.loadFromFile("images/menuSpritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexture.setMinFilter(GL_NEAREST);
	menuTexture.setMagFilter(GL_NEAREST);

	play1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	play1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY)));

	play2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.25f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	play2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY)));

	multiplayer1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	multiplayer1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y)));

	multiplayer2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.25f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	multiplayer2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y)));

	controls1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.5f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	controls1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 2)));

	controls2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.75f, 0.0f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	controls2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 2)));

	credits1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.0f, 0.25f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	credits1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 3)));

	credits2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.25f, 0.25f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	credits2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 3)));

	password1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.5f, 0.25f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	password1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 4)));

	password2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.75f, 0.25f), glm::vec2(0.25f, 0.25f), &menuTexture, &texProgram, false);
	password2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 4)));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	// Select which font you want to use
	if (!text.init("fonts/LemonMilk.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
	// Select which font you want to use

	if (!info.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update() {

	if (wrongPassword) {
		Sleep(2000);
		wrongPassword = false;
	}

	if (Game::instance().bF1) {
		Game::instance().bF1 = false;
	}
	else if (Game::instance().bF2) {
		Game::instance().bF2 = false;
	}
	if (Game::instance().bF3) {
		Game::instance().bF3 = false;
	}
	if (Game::instance().bF4) {
		Game::instance().bF4 = false;
	}
	if (Game::instance().bF5) {
		Game::instance().bF5 = false;
	}
	//Change option
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !bUpPressed && !bPassword && !bControls && !bCredits) {
		--index;
		bUpPressed = true;
	}
	else if (!Game::instance().getSpecialKey(GLUT_KEY_UP)) bUpPressed = false;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !bDownPressed && !bControls && !bPassword && !bCredits) {
		++index;
		bDownPressed = true;
	}
	else if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) bDownPressed = false;

	if (index == -1) index = numOptions - 1;
	index = index % numOptions;

	if (Game::instance().getKey(13) && !bEnterPressed) { //enter key
		MenuSound->play2D("audio/selection.mp3");
		Sleep(350);
		bEnterPressed = true;
		if (bControls) bControls = false;
		else if (bCredits) bCredits = false;
		else if (bPassword) {
			passwordAtempt = "";
			bPassword = false;
		}
		else { //Menu screen, check if options are selected
			switch (index) {
			case 0:
				Game::instance().sceneAnt = MENU;
				Game::instance().setState(LEVEL_1);
				break;
			case 1: 
				Game::instance().sceneAnt = MENU;
				Game::instance().setState(VERSUS);
				break;
			case 2:
				bControls = true;
				break;
			case 3:
				bCredits = true;
				break;
			case 4:
				Game::instance().nuevaLetra = false;
				Game::instance().borra = false;
				Game::instance().spacePressed = false;
				bPassword = true;
				break;
			}
		}
	}
	else if (!Game::instance().getKey(13)) bEnterPressed = false;

	if (bPassword) {

		if (Game::instance().nuevaLetra && passwordAtempt.size() <= 20) {
			passwordAtempt += Game::instance().letra;
			Game::instance().nuevaLetra = false;
		}
		else if (Game::instance().borra) {
			if (passwordAtempt.size() >= 1) passwordAtempt.resize(passwordAtempt.size() - 1);
			Game::instance().borra = false;
		}

		if (Game::instance().spacePressed) {
			Game::instance().spacePressed = false;
			if (passwordAtempt == password1) {
				Game::instance().sceneAnt = TELE_1;
				Game::instance().setState(ANIMATION);
				bPassword = false;
				index = 0;
				passwordAtempt = "";
			}
			else if (passwordAtempt == password2) {
				Game::instance().sceneAnt = TELE_2;
				Game::instance().setState(ANIMATION);
				bPassword = false;
				index = 0;
				passwordAtempt = "";
			}
			else if (passwordAtempt == password3) {
				Game::instance().sceneAnt = TELE_3;
				Game::instance().setState(ANIMATION);
				bPassword = false;
				index = 0;
				passwordAtempt = "";
			}
			else {
				wrongPassword = true;
				passwordAtempt = "";
			}
		}
	}
}

void Menu::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (bControls) controlsTextureQuad->render(controlsTexture);
	else if (bPassword) {
		passwordTextureQuad->render(passwordTexture);
		text.render("Introduce a password", glm::vec2(150, SCREEN_HEIGHT / 4), 30, glm::vec4(1.0f, 0.5f, 0.31f, 1));
		if (wrongPassword) text.render("WRONG PASSWORD", glm::vec2(150, SCREEN_HEIGHT / 3), 20, glm::vec4(1.0f, 0.0f, 0.0f, 1));
		else text.render(passwordAtempt, glm::vec2(150, SCREEN_HEIGHT / 3), 20, glm::vec4(1.0f, 1.0f, 1.0f, 1));
		info.render("Press START to submit password", glm::vec2(150, 420), 10, glm::vec4(1.0f, 1.0f, 1.0f, 1));
		info.render("Press ENTER to return to Menu", glm::vec2(150, 440), 10, glm::vec4(1.0f, 1.0f, 1.0f, 1));
	}
	else if (bCredits) creditsTextureQuad->render(creditsTexture);
	else {
		mainTextureQuad->render(mainTexture);
		switch (index) {
		case 0:
			play2Button->render();
			multiplayer1Button->render();
			controls1Button->render();
			credits1Button->render();
			password1Button->render();
			break;
		case 1:
			play1Button->render();
			multiplayer2Button->render();
			controls1Button->render();
			credits1Button->render();
			password1Button->render();
			break;
		case 2:
			play1Button->render();
			multiplayer1Button->render();
			controls2Button->render();
			credits1Button->render();
			password1Button->render();
			break;
		case 3:
			play1Button->render();
			multiplayer1Button->render();
			controls1Button->render();
			credits2Button->render();
			password1Button->render();
			break;
		case 4:
			play1Button->render();
			multiplayer1Button->render();
			controls1Button->render();
			credits1Button->render();
			password2Button->render();
			break;
		}
	}
}

void Menu::initShaders()
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