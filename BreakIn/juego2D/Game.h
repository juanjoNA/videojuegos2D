#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Menu.h"
#include "Scene.h"
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

	void setState(int state, bool fromPassword);

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

	int startingLevel;
	bool bF1 = false;
	bool bF2 = false;
	bool bF3 = false;
	bool bF4 = false;
	bool bF5 = false;
	bool b1 = false;
	bool b2 = false;
	bool b3 = false;

private:
	bool bPlay;                       // Continue to play game?
	int sceneAct;
	Scene scene;                      // Scene to render
	Versus sceneVersus;
	Menu menu;
	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time
	int xPlay;
	int yPlay;
};


#endif // _GAME_INCLUDE


