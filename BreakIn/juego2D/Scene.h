#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


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
	Text text;
	glm::ivec2 ballPos;
	ShaderProgram simpleProgram, texProgram;
	glm::mat4 projection;
	vector<Element> bricks, money, objectsInGame, objVector;
	Element *key, *alarm;
	Police *police;
	float translation;
	int subnivel;
	float currentTime;
	float yAnterior = 0;
};


#endif // _SCENE_INCLUDE

