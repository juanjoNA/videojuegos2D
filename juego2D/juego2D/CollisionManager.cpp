#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CollisionManager.h"
#include "TileMap.h"

bool CollisionManager::collisionBallMap(glm::ivec2 &pos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const
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

bool CollisionManager::collisionBallPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const
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
		if (oldPos.y < pos.y) {
			/*
			if (xmax <= player->getPosition().x + 4 || (player->getPosition().x + player->getSize().x - 4) <= xmin) {	//Golpea en el lateral izquierdo o derecho
				velocitat.y = -2.0f;	
			}
			
			else if ((xmax > player->getPosition().x + 4 && xmax < player->getPosition().x + 20) ||
				((player->getPosition().x + player->getSize().x - 4) < xmin) && ((player->getPosition().x + player->getSize().x - 20) > xmin)
				) //Golpea un poco a la izquierda o la derecha en la paleta 
			{
				velocitat.y = -3.0f; 
			}
			else if ( (xmax > player->getPosition().x + ((player->getSize().x) / 2) - 1 && xmax < player->getPosition().x + ((player->getSize().x) / 2) - size.x) ) {
				velocitat.x = -2.f; //Golpea en el centro por la izquierda
				velocitat.y = -velocitat.y;
			}
			else if ((xmin < player->getPosition().x + ((player->getSize().x) / 2) + 1 && xmax < player->getPosition().x + ((player->getSize().x) / 2) + size.x)) {
				velocitat.x = 2.f;		//Golpea en el centro por la derecha
				velocitat.y = -velocitat.y;
			}
			else {
				velocitat.x = -velocitat.x;
				velocitat.y = -velocitat.y;
			}*/
			velocitat.y = -velocitat.y;
			return true;
		}
		else return false;
		
	}

	return false;
	return false;
}

bool CollisionManager::collisionPlayerMap(glm::ivec2 &pos, const glm::ivec2 & size, TileMap *tileMap, glm::ivec2 direction) const
{
	int xmin = pos.x / tileMap->getTileSize();
	int ymin = pos.y / tileMap->getTileSize();
	int xmax = (pos.x + size.x) / tileMap->getTileSize();
	int ymax = (pos.y + size.y) / tileMap->getTileSize();
	int *map = tileMap->getMap();

	if (direction.x < 0) xmin = (pos.x+direction.x) / tileMap->getTileSize();
	else if (direction.x > 0) xmax = (pos.x + size.x + direction.x) / tileMap->getTileSize();
	else if (direction.y < 0) ymin = (pos.y + direction.y) / tileMap->getTileSize();
	else ymax = (pos.y + size.y + direction.y) / tileMap->getTileSize();

	int topLeft = map[(ymin * tileMap->getMapSize().x) + xmin];
	int topRight = map[(ymin * tileMap->getMapSize().x) + xmax];
	int bottomLeft = map[(ymax * tileMap->getMapSize().x) + xmin];
	int bottomRight = map[(ymax * tileMap->getMapSize().x) + xmax];

	if ((topLeft <= 3) || (topRight <= 3) || (bottomLeft <= 3) || (bottomRight <= 3)) return true;
	else return false;
}

/*bool CollisionManager::collisionObjects(glm::ivec2 & pos, glm::ivec2 & oldPos, const glm::ivec2 & size, vector<Component> components, glm::vec2 &velocitat) const
{
	for (int i = 0; i < components.size(); i++) {
		int xmin = pos.x;
		int xmax = pos.x + size.x;
		int ymin = pos.y;
		int ymax = pos.y + size.y;

		if	(
			((components.at(i).getPosition().x + components.at(i).getSize().x) >= xmin) &&
			(xmax >= components.at(i).getPosition().x) &&
			((components.at(i).getPosition().y + components.at(i).getSize().y) >= ymin) &&
			(ymax >= components.at(i).getPosition().y)
			) 
		{
			components.at(i).collision();
			
		}
	}
}
*/