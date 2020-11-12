#ifndef _VERSUS_INCLUDE
#define _VERSUS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Element.h"
#include "Police.h"
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Versus
{

public:
	Versus();
	~Versus();

	void init();
	void update(int deltaTime);
	void render();
	void reinit(int playerPos);

private:
	void initShaders();
	void goal(int jugador);
	void winner(int jugador);

private:
	TileMap *map;
	TexturedQuad* goalExplanationTextureQuad;
	Texture win;
	Player *player1, *player2;
	Ball *ball;
	Texture titlesTexture;
	Sprite *goals1, goals2;
	Text text;
	Text info;
	bool startMessage;
	ShaderProgram simpleProgram, texProgram;
	glm::mat4 projection;
	float currentTime;
	int res1, res2, playerWin;
};


#endif // _SCENE_INCLUDE

