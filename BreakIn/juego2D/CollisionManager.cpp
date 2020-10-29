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
}

bool CollisionManager::collisionBallPlayer(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const
{
	
	int xmin = pos.x;
	int xmax = pos.x + size.x;
	int ymin = pos.y;
	int ymax = pos.y + size.y;
	glm::vec2 center = glm::vec2((xmax+xmin)/2, (ymax+ymin)/2);

	if (
		((player->getPosition().x + player->getSize().x) >= xmin) &&
		(xmax >= player->getPosition().x) &&
		((player->getPosition().y + player->getSize().y) >= ymin) &&
		(ymax >= player->getPosition().y)
		)
	{
		if (oldPos.y < pos.y) {
			
			velocitat.y = -velocitat.y;
			float percentatgeCollision = ( (player->getPosition().x + player->getSize().x) - center.x ) / player->getSize().x;

			if (percentatgeCollision <= 0.2f) {
				velocitat.x = 5.f;
			}
			else if (percentatgeCollision > 0.2f && percentatgeCollision <= 0.35f) {
				velocitat.x = 3.f;
			}
			else if (percentatgeCollision > 0.35f && percentatgeCollision < 0.5f) {
				velocitat.x = 1.f;
			}
			else if (percentatgeCollision == 0.5f) {
				velocitat.x = 0;
			}
			else if (percentatgeCollision > 0.5f && percentatgeCollision <= 0.65f) {
				velocitat.x = -1.f;
			}
			else if (percentatgeCollision > 0.65f && percentatgeCollision <= 0.8f) {
				velocitat.x = -3.f;
			}
			else {
				velocitat.x = -5.f;
			}
			return true;
		}
		else return false;
	}
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

bool CollisionManager::collisionObjects(glm::ivec2 & pos, glm::ivec2 & oldPos, const glm::ivec2 & size, vector<class Element>& elements, glm::vec2 &velocitat) const
{
	for (int i = 0; i < elements.size(); i++) {
		int xmin = pos.x;
		int xmax = pos.x + size.x;
		int ymin = pos.y;
		int ymax = pos.y + size.y;

		if	(
			((elements.at(i).getPosition().x + elements.at(i).getSize().x) >= xmin) &&
			(xmax >= elements.at(i).getPosition().x) &&
			((elements.at(i).getPosition().y + elements.at(i).getSize().y) >= ymin) &&
			(ymax >= elements.at(i).getPosition().y)
			)
		{
			int resistance = elements.at(i).collision();
			if (resistance == 0) {
				//elements.erase(elements.begin()+i);
			}
			velocitat = -velocitat;
			return true;
		}
	}

	return false;
}
