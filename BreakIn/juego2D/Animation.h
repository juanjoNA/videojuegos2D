
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Animation
{

public:
	Animation();
	~Animation();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	Texture controlsTexture;
	Texture gameOverTexture, transition1Texture, transition2Texture, winTexture, initialTexture;
	Sprite *gameOver, *thief;
	Scene *scene;
	TexturedQuad *TextureQuad;
	Text text;
	float currentTime;
	glm::mat4 projection;

};