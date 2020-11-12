#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Menu.h"
#include "Scene.h"
#include "Animation.h"
#include "Versus.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}


	static Game &instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	void setState(int state);

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	int sceneAnt;
	int points, money, lives;
	bool bF1 = false;
	bool bF2 = false;
	bool bF3 = false;
	bool bF4 = false;
	bool bF5 = false;
	bool b1 = false;
	bool b2 = false;
	bool b3 = false;
	bool godMode = false;
	bool destroyBlocks = false;
	bool pickMoney = false;
	bool gameOver = false;
	char letra;
	bool nuevaLetra = false;
	bool borra = false;
	bool spacePressed = false;
	bool enterPressed = false;

	void initGamePropierties();

private:
	bool bPlay;                       // Continue to play game?
	TexturedQuad* creditsTextureQuad;
	Texture creditsTexture;
	int sceneAct;
	Scene scene;                      // Scene to render
	Menu menu;
	Animation animation;
	Versus sceneVersus;
	ShaderProgram texProgram;
	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time
	int xPlay;
	int yPlay;
};


#endif // _GAME_INCLUDE


