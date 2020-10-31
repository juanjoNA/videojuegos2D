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

void Menu::init() {
	initShaders();

	index = 0; //Option by default
	numOptions = 4;
	bUpPressed = false;
	bDownPressed = false;
	bEnterPressed = false;

	bCredits = false;
	bPassword = false;
	bControls = false;

	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };

	mainTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	mainTexture.loadFromFile("images/fondoBreakIn.png", TEXTURE_PIXEL_FORMAT_RGB);

	controlsTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	controlsTexture.loadFromFile("images/controls.png", TEXTURE_PIXEL_FORMAT_RGB);

	creditsTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	creditsTexture.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);

	passwordTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	passwordTexture.loadFromFile("images/fondoBreakIn.png", TEXTURE_PIXEL_FORMAT_RGB);

	menuTexture.loadFromFile("images/menuSpritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTexture.setMinFilter(GL_NEAREST);
	menuTexture.setMagFilter(GL_NEAREST);

	play1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	play1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY)));

	play2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.25f, 0.0f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	play2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY)));

	controls1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.5f, 0.0f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	controls1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y)));

	controls2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.75f, 0.0f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	controls2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y)));

	credits1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.0f, 0.5f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	credits1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 2)));

	credits2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.25f, 0.5f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	credits2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 2)));

	password1Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	password1Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 3)));

	password2Button = Sprite::createSprite(glm::ivec2(BUTTON_X, BUTTON_Y), glm::vec2(0.75f, 0.5f), glm::vec2(0.25f, 0.5f), &menuTexture, &texProgram, false);
	password2Button->setPosition(glm::vec2(float(SCREEN_WIDTH / 2 - BUTTON_X / 2), float(firstButtonY + BUTTON_Y * 3)));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

}

void Menu::update() {
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
		if (bControls) //At instructions screen
			bControls = false; //Exit instructions screen
		else if (bCredits)
			bCredits = false; //May be different for records!
		else if (bPassword)
			bPassword = false;
		else { //Menu screen, check if options are selected
			switch (index) {
			case 0: //start game
				Game::instance().setState(GAME);
				break;
			case 1: //show instructions
				bControls = true;
				break;
			case 2:
				bPassword = true;
				break;
			case 3: //show credits
				bCredits = true;
				break;
			}
		}
	}
	else if (!Game::instance().getKey(13)) bEnterPressed = false;
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
	else if (bPassword) passwordTextureQuad->render(passwordTexture);
	else if (bCredits) creditsTextureQuad->render(creditsTexture);
	else {
		mainTextureQuad->render(mainTexture);
		switch (index) { //This only renders the actual "selected" text with different color
		case 0:
			play2Button->render();
			controls1Button->render();
			credits1Button->render();
			password1Button->render();
			//playText.render("*", glm::vec2(screen_width*0.08, screen_height*0.57), text_size, glm::vec4(1, 1, 1, 1));
			break;
		case 1:
			play1Button->render();
			controls2Button->render();
			credits1Button->render();
			password1Button->render();
			//instructionsText.render("*", glm::vec2(screen_width*0.08, screen_height*0.73), text_size, glm::vec4(1, 1, 1, 1));
			break;
		case 2:
			play1Button->render();
			controls1Button->render();
			credits2Button->render();
			password1Button->render();
			//recordsText.render("*", glm::vec2(screen_width*0.08, screen_height*0.89), text_size, glm::vec4(1, 1, 1, 1));
			break;
		case 3:
			play1Button->render();
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