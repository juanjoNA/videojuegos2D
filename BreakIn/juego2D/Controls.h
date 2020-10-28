#pragma once

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"

class Controls {
public:
	Controls() {}

	static Controls &instance()
	{
		static Controls C;

		return C;
	}

	void init();
	void update();
	void render();
	void mouseClick(int x, int y);

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	TexturedQuad* controlsTextureQuad; //GUI Background
	Texture controlsTexture;
};