#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	Sprite() = default;
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &posInSpritesheet, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, bool isRepeatable);

	Sprite(const glm::vec2 &quadSize, const glm::vec2 &posInSpritesheet, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, bool isRepeatable);

	void update(int deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	int animation() const;
	
	void setPosition(const glm::vec2 &pos);
	int getCurrentAnimation(){ return currentAnimation; }
	bool isFinished();

private:
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position;
	int currentAnimation, currentKeyframe, lastKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;
	bool animate, repeat, finish;
};


#endif // _SPRITE_INCLUDE

