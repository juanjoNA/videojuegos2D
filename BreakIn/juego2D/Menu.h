#pragma once

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"

class Menu {
public:
	Menu() {}

	static Menu &instance()
	{
		static Menu M;

		return M;
	}

	void init();
	void update();
	void render();
	void mouseClick(int x, int y);

private:
	void initShaders();

private:
	int index;
	int numOptions;
	bool bUpPressed, bDownPressed, bEnterPressed;
	bool bCredits; //Go to credits screen
	bool bControls; //Go to instructions screen
	bool bPassword; //Go to password screen

	ShaderProgram texProgram;
	glm::mat4 projection;
	TexturedQuad* mainTextureQuad; //GUI Background
	TexturedQuad* controlsTextureQuad; //GUI Background
	TexturedQuad* creditsTextureQuad; //GUI Background
	TexturedQuad* passwordTextureQuad; //GUI Background
	Texture mainTexture;
	Texture controlsTexture;
	Texture creditsTexture;
	Texture passwordTexture;
	Texture menuTexture;
	Sprite *play1Button;
	Sprite *controls1Button;
	Sprite *credits1Button;
	Sprite *password1Button;
	Sprite *play2Button;
	Sprite *controls2Button;
	Sprite *credits2Button;
	Sprite *password2Button;
};