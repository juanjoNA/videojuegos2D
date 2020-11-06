#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CollisionManager.h"
#include "TileMap.h"
#include "Misil.h"
#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *CollisionSound = createIrrKlangDevice();

bool CollisionManager::collisionBallMap(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, const TileMap *tileMap, glm::vec2 &velocitat) const
{
	float xmin = pos.x / float(tileMap->getTileSize());
	float ymin = pos.y / float(tileMap->getTileSize());
	float xmax = (pos.x + size.x) / float(tileMap->getTileSize());
	float ymax = (pos.y + size.y) / float(tileMap->getTileSize());
	int *map = tileMap->getMap();

	int topLeft = map[(int(ymin) * tileMap->getMapSize().x) + int(xmin)];
	int topRight = map[(int(ymin) * tileMap->getMapSize().x) + int(xmax)];
	int bottomLeft = map[(int(ymax) * tileMap->getMapSize().x) + int(xmin)];
	int bottomRight = map[(int(ymax) * tileMap->getMapSize().x) + int(xmax)];

	if ((topLeft <= 3 && topRight <= 3) || (bottomLeft <= 3 && bottomRight <= 3))
	{
		velocitat.y = -velocitat.y;
		if ((topLeft <= 3 && bottomLeft <= 3) || (topRight <= 3 && bottomRight <= 3)) velocitat.x = -velocitat.x;
		CollisionSound->play2D("audio/bounce.wav");
		return true;
	}
	else if ((topLeft <= 3 && bottomLeft <= 3) || (topRight <= 3 && bottomRight <= 3))
	{
		velocitat.x = -velocitat.x;
		if ((topLeft <= 3 && topRight <= 3) || (bottomLeft <= 3 && bottomRight <= 3)) velocitat.y = -velocitat.y;
		CollisionSound->play2D("audio/bounce.wav");
		return true;
	}
	else if ((topLeft <= 3) || (topRight <= 3) || (bottomLeft <= 3) || (bottomRight <= 3))
	{
		glm::ivec2 posCollision, auxPos = pos, auxOldPos = oldPos;
		float x,y;

		float entera;
		float decimalX, decimalY; //decimales de los dos ejes

		for (int i = 0; i < 3; i++) {
			posCollision = (auxPos + auxOldPos) / 2;
			x = posCollision.x / float(tileMap->getTileSize());
			y = posCollision.y / float(tileMap->getTileSize());
			if (map[(int(y) * tileMap->getMapSize().x) + int(x)] <= 3) auxPos = posCollision;
			else auxOldPos = posCollision;
		}


		decimalX = modf(x, &entera);
		decimalY = modf(y, &entera);

		if (topLeft <= 3) {
			if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
			else if(bottomRight <= 3) velocitat = -velocitat;
			else {
				if (decimalX > decimalY) velocitat.x = -velocitat.x;
				else if (decimalY > decimalX)  velocitat.y = -velocitat.y;
				else velocitat = -velocitat;
			}
		}
		else if (topRight <= 3) {
			if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
			else if (bottomLeft <= 3) velocitat = -velocitat;
			else {
				decimalY = 1 - decimalY;
				if (decimalX == 0) velocitat.x = -velocitat.x;
				else if (decimalX > decimalY) velocitat.y = -velocitat.y;
				else if (decimalY > decimalX)  velocitat.x = -velocitat.x;
				else velocitat = -velocitat;
			}
		}
		else if (bottomLeft <= 3) {
			if (decimalY == 0) velocitat.y = -velocitat.y;
			else if(decimalX > decimalY) velocitat.x = -velocitat.x;
			else if(decimalY > decimalX) velocitat.y = -velocitat.y;
			else velocitat = -velocitat;
		}
		else {
			if (oldPos.y > pos.y) velocitat.x = -velocitat.x;
			else if (topRight <= 3) velocitat = -velocitat;
			else {
				decimalY = 1 - decimalY;
				if (decimalX > decimalY) velocitat.y = -velocitat.y;
				else if (decimalY > decimalX)  velocitat.x = -velocitat.x;
				else velocitat = -velocitat;
			}
		}
		CollisionSound->play2D("audio/bounce.wav");
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
	glm::vec2 center = glm::vec2((xmax + xmin) / 2, (ymax + ymin) / 2);

	if (
		((player->getPosition().x + player->getSize().x) >= xmin) &&
		(xmax >= player->getPosition().x) &&
		((player->getPosition().y + player->getSize().y) >= ymin) &&
		(ymax >= player->getPosition().y)
		)
	{
		if (oldPos.y < pos.y) {

			velocitat.y = -velocitat.y;
			float percentatgeCollision = ((player->getPosition().x + player->getSize().x) - center.x) / player->getSize().x;

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
			CollisionSound->play2D("audio/bounce.wav");
			return true;
		}
		else return false;
	}
	return false;
}

bool CollisionManager::collisionPlayerMap(glm::ivec2 &pos, const glm::ivec2 & size, TileMap *tileMap, glm::ivec2 direction) const
{
	/*int xmin = pos.x / tileMap->getTileSize();
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
	else return false;*/

	if ((pos.x + direction.x < 16) || (pos.x + size.x + direction.x > 432) &&
		(pos.y + direction.y < 16) || (pos.y + size.y + direction.y > 432)) {
		return true;
	}else return false;
}

bool CollisionManager::collisionObjects(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, vector<class Element>& elements, glm::vec2 &velocitat) const
{
	for (int i = 0; i < elements.size(); i++) {
		int xmin = pos.x;
		int xmax = pos.x + size.x;
		int ymin = pos.y;
		int ymax = pos.y + size.y;
		int posX = elements.at(i).getPosition().x;
		int posY = elements.at(i).getPosition().y;
		int sizeX = elements.at(i).getSize().x;
		int sizeY = elements.at(i).getSize().y;
		if (
			((posX + sizeX) >= xmin) &&
			(xmax >= posX) &&
			((posY + sizeY) >= ymin) &&
			(ymax >= posY)
			)
		{
			int resistance = elements.at(i).collision();
			int type = elements.at(i).getType();
			glm::vec2 c = glm::vec2((pos.x + pos.x + size.x) / 2, (pos.y + pos.y + size.y) / 2);

			if (c.x >= posX && c.x <= posX + sizeX) velocitat.y = -velocitat.y;			//GOLPEA POR ARRIBA O POR ABAJO
			else if(c.y >= posY && c.y <= posY + sizeY) velocitat.x = -velocitat.x;		//GOLPEA POR UN LATERAL
			else {
				float difX, difY;
				if (c.x < posX) {
					difX = posX - c.x;
					if (c.y < posY) difY = posY - c.y;		//CUADRANTE SUPERIOR IZQUIERDO
					else difY = c.y - posY+sizeY;			//CUADRANTE INFERIOR IZQUIERDO
				}
				else {
					difX = posX+sizeX - c.x;
					if (c.y < posY) difY = posY - c.y;		//CUADRANTE SUPERIOR DERECHO
					else difY = c.y - posY+sizeY;					//CUADRANTE INFERIOR DERECHO
				}

				if (difX < difY) velocitat.y = -velocitat.y;
				else if (difX > difY) velocitat.x = -velocitat.x;
				else velocitat = -velocitat;
			}
			
			
			
			
			
			
			
			if (type == 0 && elements.at(i).getResistance() == 0) {
				//Brick
				CollisionSound->play2D("audio/brickBreak.wav");
			}
			else if (type == 0) {
				CollisionSound->play2D("audio/brick.mp3");
			}
			else if (type == 1) {
				//Money
				CollisionSound->play2D("audio/coin.wav");
			}
			else if (type == 2) {
				//Alarm
				CollisionSound->play2D("audio/alarm.mp3");
			}
			else {
				//Key
				CollisionSound->play2D("audio/key.mp3");
			}
			return true;
		}
	}

	return false;
}

int CollisionManager::collisionMisil(glm::ivec2 &posMisil, const glm::ivec2 &sizeMisil, Player *player) const
{
	int xmin = posMisil.x;
	int xmax = posMisil.x + sizeMisil.x;
	int ymin = posMisil.y;
	int ymax = posMisil.y + sizeMisil.y;

	if (
		((player->getPosition().x + player->getSize().x) >= xmin) &&
		(xmax >= player->getPosition().x) &&
		((player->getPosition().y + player->getSize().y) >= ymin) &&
		(ymax >= player->getPosition().y)
		) {
		return 1;
	}
	else if (ymax == 432) return 2;
	else return 0;
}
