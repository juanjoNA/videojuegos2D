#pragma once

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"

class Menu {
public:

	static Menu &instance()
	{
		static Menu M;

		return M;
	}

	Menu();

	void init();
	void update();
	void render();

private:
	void initShaders();

private:
	int index;
	int numOptions;
	bool bUpPressed, bDownPressed, bEnterPressed;
	bool bCredits;
	bool bControls;
	bool bPassword;

	ShaderProgram texProgram;
	glm::mat4 projection;
	Texture titlesTexture;
	TexturedQuad* mainTextureQuad;
	TexturedQuad* controlsTextureQuad;
	TexturedQuad* creditsTextureQuad;
	TexturedQuad* passwordTextureQuad;
	Texture mainTexture;
	Texture controlsTexture;
	Texture creditsTexture;
	Texture passwordTexture;
	Texture menuTexture;
	Text text;
	Sprite *play1Button;
	Sprite *controls1Button;
	Sprite *credits1Button;
	Sprite *password1Button;
	Sprite *play2Button;
	Sprite *controls2Button;
	Sprite *credits2Button;
	Sprite *password2Button;
	string password1;
};