#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CollisionManager.h"
#include "TileMap.h"

bool CollisionManager::collisionMap(glm::ivec2 &pos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const
{
	float xmin = pos.x / float( tileMap->getTileSize() );
	float ymin = pos.y / float( tileMap->getTileSize() );
	float xmax = (pos.x + size.x) / float( tileMap->getTileSize() );
	float ymax = (pos.y + size.y) / float( tileMap->getTileSize() );
	int *map = tileMap->getMap();

	float entera;
	float decimalX, decimalY; //decimales de los dos ejes

	decimalX = modf(xmin, &entera);
	decimalY = modf(ymin, &entera);

	int topLeft = map[(int(ymin) * tileMap->getMapSize().x) + int(xmin)];
	int topRight = map[(int(ymin) * tileMap->getMapSize().x) + int(xmax)];
	int bottomLeft = map[(int(ymax) * tileMap->getMapSize().x) + int(xmin)];
	int bottomRight = map[(int(ymax) * tileMap->getMapSize().x) + int(xmax)];

	if ( (topLeft <= 3 && topRight <= 3) || (bottomLeft <= 3 && bottomRight <= 3) )
	{
		velocitat.y = -velocitat.y;
		return true;
	}
	else if ((topLeft <= 3 && bottomLeft <= 3) || (topRight <= 3 && bottomRight <= 3))
	{
		velocitat.x = -velocitat.x;
		return true;
	}
	else if ( (topLeft <= 3)  || (topRight <= 3) || (bottomLeft <= 3) || (bottomRight <= 3) )
	{
		if (topLeft <= 3) {
			if (decimalX > decimalY) velocitat.x = -velocitat.x;
			else if (decimalY > decimalX) velocitat.y = -velocitat.y;
			else velocitat = -velocitat;
		}
		else if (topRight <= 3) {
			if(decimalX == 0) velocitat.x = -velocitat.x;
			else if (decimalX > decimalY) velocitat.x = -velocitat.x;
			else if (decimalY > decimalX) velocitat.y = -velocitat.y;
			else velocitat = -velocitat;
		}
		else if (bottomLeft <= 3) {
			if (decimalY == 0) velocitat.y = -velocitat.y;
			else if(decimalX > decimalY) velocitat.x = -velocitat.x;
			else if(decimalY > decimalX) velocitat.y = -velocitat.y;
			else velocitat = -velocitat;
		}
		else {
			if (decimalX > decimalY) velocitat.y = -velocitat.y;
			else if (decimalY > decimalX) velocitat.x = -velocitat.x;
			else velocitat = -velocitat;
		}
		
		return true;
	}
	return false;
	/*
	glm::vec2 vector = oldPos - pos;

	/*the ball is drawn above, and I have mapped out points around the ball to check for collision
	once 7 points on the circle are touching, we get xNormal and yNormal.
	i add the coordinates of these points (relative to center of ball)
	and average them in order to get the best point from which to calculate the normal.
	these numbers are calculated properly and return the desired numbers
	xNormal = -xNormal / 7;
	yNormal = -yNormal / 7;

	//velocity (it works)
	v = sqrt(xin*xin + yin*yin);
	printf("v: %f\n\n", v);
	//calculate theta
	theta = atan(yin / xin);
	printf("theta: %f\n\n", theta);
	//and beta...
	beta = atan(yNormal / xNormal);

	printf("beta: %f\n\n", beta);

	//A should be the angle at which to send the ball off
	A = 2 * beta + theta;

	//calculate the velocity at which to send the ball in each direction, updated each frame
	yin = sin(A) * v;
	xin = cos(A) * v;
	*/
}

bool CollisionManager::collisionPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const
{
	glm::vec2 vecVelocidad = pos - oldPos;
	int xmin = pos.x;
	int xmax = pos.x + size.x;
	int ymin = pos.y;
	int ymax = pos.y + size.y;
	

	if (
		((player->getPosition().x + player->getSize().x) >= xmin) &&
		(xmax >= player->getPosition().x) &&
		((player->getPosition().y + player->getSize().y) >= ymin) &&
		(ymax >= player->getPosition().y)
		)
	{
		velocitat.y = -velocitat.y;
		return true;
	}

	return false;
	return false;
}
