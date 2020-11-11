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
	void reinit(int reinitLevel);
	int level;

private:
	void initShaders();
	void storeObjects();
	bool loadObjects(const string &loadObjectsLevel);
	void createBricks1();
	void createBricks2();
	void createBricks3();
	void createMoney();
	void createKey();
	void createAlarm();
	void createDoor();
	void setGameTextPosition(int subnivel);
	void pierdeVida();
	void subeNivel();

private:
	TileMap *map, *map1, *map2, *map3;
	Player *player;
	Ball *ball;
	Texture tapadorTexture;
	Texture titlesTexture;
	Texture gameOverTexture;
	Sprite *tapadorArriba;
	Sprite *tapadorAbajo;
	Sprite *moneyTitle;
	Sprite *levelTitle;
	Sprite *pointsTitle;
	Sprite *roomTitle;
	Sprite *livesTitle;
	Text text;
	glm::ivec2 ballPos;
	glm::ivec2 playerPos;
	ShaderProgram simpleProgram, texProgram;
	glm::mat4 projection;
	vector<Element> objectsInGame, objVector;
	Element *key, *alarm, *door;
	Police *police;
	float translation;
	int subnivel;
	float currentTime;
	float yAnterior = 0;
	int money, lives, points;
	int timeRef;
};


#endif // _SCENE_INCLUDE

