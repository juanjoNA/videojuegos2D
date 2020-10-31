#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Element.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void createObjects();
	void createBricks1();
	void createBricks2();
	void createBricks3();
	void createMoney();
	void createKey();
	void createAlarm();

private:
	TileMap *map;
	Player *player;
	Ball *ball;
	Texture tapadorTexture;
	Sprite *tapadorArriba;
	Sprite *tapadorAbajo;
	glm::ivec2 ballPos;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	vector<Element> bricks, money;
	float translation;
	int subnivel;
};


#endif // _SCENE_INCLUDE

