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

		float difX, difY; //decimales de los dos ejes

		//calculo el centro de la pelota
		glm::vec2 c = glm::vec2((pos.x + pos.x + size.x) / 2, (pos.y + pos.y + size.y) / 2);

		if (topLeft <= 3) {
			if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
			else if (oldPos.x < pos.x) velocitat.y = -velocitat.y;
			else if (bottomRight <= 3) velocitat = -velocitat;
			else {
				difX = c.x - (int(xmin) * tileMap->getTileSize() + tileMap->getTileSize());
				difY = c.y - (int(ymin) * tileMap->getTileSize() + tileMap->getTileSize());
				if (difX > difY) velocitat.x = -velocitat.x;
				else if (difY > difX)  velocitat.y = -velocitat.y;
				else velocitat = -velocitat;
			}
		}
		else if (topRight <= 3) {
			if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
			else if (oldPos.x > pos.x) velocitat.y = -velocitat.y;
			else if (bottomLeft <= 3) velocitat = -velocitat;
			else {
				difX = (int(xmax) * tileMap->getTileSize()) - c.x;
				difY = c.y - (int(ymin) * tileMap->getTileSize() + tileMap->getTileSize());
				if (difY == 0) velocitat.x = -velocitat.x;
				else if (difX > difY) velocitat.x = -velocitat.x;
				else if (difY > difX)  velocitat.y = -velocitat.y;
				else velocitat = -velocitat;
			}
		}
		else if (bottomLeft <= 3) {
			if (oldPos.y > pos.y) velocitat.x = -velocitat.x;
			else if (oldPos.x < pos.x) velocitat.y = -velocitat.y;
			else if (topRight <= 3) velocitat = -velocitat;
			else {
				difX = c.x - (int(xmin) * tileMap->getTileSize() + tileMap->getTileSize());
				difY = (int(ymax) * tileMap->getTileSize()) - c.y;
				if (difX > difY) velocitat.x = -velocitat.x;
				else if (difY > difX)  velocitat.y = -velocitat.y;
				else velocitat = -velocitat;
			}
		}
		else {
			if (oldPos.y > pos.y) velocitat.x = -velocitat.x;
			else if (oldPos.x > pos.x) velocitat.y = -velocitat.y;
			else if (topRight <= 3) velocitat = -velocitat;
			else {
				difX = (int(xmax) * tileMap->getTileSize()) - c.x;
				difY = (int(ymax) * tileMap->getTileSize()) - c.y;
				if (difX > difY) velocitat.x = -velocitat.x;
				else if (difY > difX)  velocitat.y = -velocitat.y;
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

bool CollisionManager::collisionBallPlayerVersus(glm::ivec2 &pos, const glm::ivec2 &size, Player *player, glm::vec2 &velocitat) const
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

		velocitat.x = -velocitat.x;
		float percentatgeCollision = ((player->getPosition().y + player->getSize().y) - center.y) / player->getSize().y;

		if (percentatgeCollision <= 0.2f) {
			velocitat.y = 7.f;
		}
		else if (percentatgeCollision > 0.2f && percentatgeCollision <= 0.35f) {
			velocitat.y = 5.f;
		}
		else if (percentatgeCollision > 0.35f && percentatgeCollision < 0.5f) {
			velocitat.y = 3.f;
		}
		else if (percentatgeCollision == 0.5f) {
			velocitat.y = 0;
		}
		else if (percentatgeCollision > 0.5f && percentatgeCollision <= 0.65f) {
			velocitat.y = -3.f;
		}
		else if (percentatgeCollision > 0.65f && percentatgeCollision <= 0.8f) {
			velocitat.y = -5.f;
		}
		else {
			velocitat.y = -7.f;
		}
		CollisionSound->play2D("audio/bounce.wav");
		return true;
	}
	return false;
}

bool CollisionManager::collisionPlayerMap(glm::ivec2 &pos, int subnivel, const glm::ivec2 & size, TileMap *tileMap, glm::ivec2 direction) const
{
	int ymin, ymax;
	if (subnivel >= 0) {
		ymin = 16 + (432 * (3 - subnivel) + ((3 - subnivel) * 16));
		ymax = 432 + (ymin - 16);

		if ((pos.x + direction.x < 16) || (pos.x + size.x + direction.x > 368) ||
			(pos.y + direction.y < ymin) || (pos.y + size.y + direction.y > ymax)) return true;

		return false;
	}
	else {
		ymin = tileMap->getTileSize();
		ymax = tileMap->getMapSize().y * tileMap->getTileSize() - tileMap->getTileSize();
		switch (subnivel) 
		{
			case -1: //player1 del 1v1
				if ((pos.x + direction.x < tileMap->getTileSize()) || (pos.x + size.x + direction.x > (tileMap->getMapSize().x*tileMap->getTileSize() /2)-tileMap->getTileSize()) ||
					(pos.y + direction.y < ymin) || (pos.y + size.y + direction.y > ymax)) return true;
				break;
			case -2: //player2 del 1v1
				if ((pos.x + direction.x < (tileMap->getMapSize().x*tileMap->getTileSize() / 2) + tileMap->getTileSize()) || (pos.x + size.x + direction.x > tileMap->getMapSize().x*tileMap->getTileSize() -tileMap->getTileSize()) ||
					(pos.y + direction.y < ymin) || (pos.y + size.y + direction.y > ymax)) return true;
				break;
		}
		return false;
	}

}

bool CollisionManager::collisionObjects(glm::ivec2 &pos, glm::ivec2 &oldPos, const glm::ivec2 &size, vector<class Element>& elements, glm::vec2 &velocitat) const
{
	vector<int> eCollision;
	bool colision = false;

	for (int i = 0; i < elements.size(); i++) {
		if (hayColision(pos, size, elements.at(i).getPosition(), elements.at(i).getSize())) {
			eCollision.push_back(i);
			if(!colision) colision = true;
		}
	}
		
	if (colision) {
		glm::vec2 cBall, cE, posE, sizeE;
		float d=1000, dAux, difX, difY;
		int posicionVec, elemPos, type;

		cBall = glm::vec2( (pos.x + pos.x + size.x) / 2, (pos.y + pos.y + size.y) / 2);
		

		for (int j = 0; j < eCollision.size(); j++) {
			posicionVec = eCollision.at(j);
			posE = elements.at(posicionVec).getPosition();
			sizeE = elements.at(posicionVec).getSize();
			cE = glm::vec2((posE.x + posE.x + sizeE.x) / 2, (posE.y + posE.y + sizeE.y) / 2);
			dAux = glm::distance(cBall, cE);

			if (dAux < d) {
				elemPos = eCollision.at(j);
				d = dAux;
			}
		}
		elements.at(elemPos).collision();

		posE = elements.at(elemPos).getPosition();
		sizeE = elements.at(elemPos).getSize();
		type = elements.at(elemPos).getType();

		if (cBall.x >= posE.x && cBall.x <= posE.x + sizeE.x) velocitat.y = -velocitat.y;			//GOLPEA POR ARRIBA O POR ABAJO
		else if (cBall.y >= posE.y && cBall.y <= posE.y + sizeE.y) velocitat.x = -velocitat.x;		//GOLPEA POR UN LATERAL
		else {
			bool topLeft, topRight, bottomLeft, bottomRight;
			topLeft = (pos.x <= posE.x+sizeE.x && pos.x > posE.x && pos.y >= posE.y && pos.y <= posE.y+sizeE.y);
			topRight = (pos.x < posE.x && pos.x+size.x >= posE.x && pos.y >= posE.y && pos.y <= posE.y + sizeE.y);
			bottomLeft = (pos.x <= posE.x + sizeE.x && pos.x > posE.x && pos.y+size.y >= posE.y && pos.y+size.y <= posE.y + sizeE.y);
			bottomRight = (pos.x < posE.x && pos.x + size.x >= posE.x && pos.y+size.y >= posE.y && pos.y+size.y <= posE.y + sizeE.y);;

			if (topLeft) {
				if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
				else if (oldPos.x < pos.x) velocitat.y = -velocitat.y;
				else if (bottomRight) velocitat = -velocitat;
				else {
					difX = cBall.x - posE.x;
					difY = cBall.y - posE.y;
					if (difX > difY) velocitat.x = -velocitat.x;
					else if (difY > difX)  velocitat.y = -velocitat.y;
					else velocitat = -velocitat;
				}
			}
			else if (topRight) {
				if (oldPos.y < pos.y) velocitat.x = -velocitat.x;
				else if (oldPos.x > pos.x) velocitat.y = -velocitat.y;
				else if (bottomLeft) velocitat = -velocitat;
				else {
					difX = posE.x+sizeE.x - cBall.x;
					difY = cBall.y - posE.y;
					if (difY == 0) velocitat.x = -velocitat.x;
					else if (difX > difY) velocitat.x = -velocitat.x;
					else if (difY > difX)  velocitat.y = -velocitat.y;
					else velocitat = -velocitat;
				}
			}
			else if (bottomLeft) {
				if (oldPos.y > pos.y) velocitat.x = -velocitat.x;
				else if (oldPos.x < pos.x) velocitat.y = -velocitat.y;
				else if (topRight) velocitat = -velocitat;
				else {
					difX = cBall.x - posE.x;
					difY = posE.y+sizeE.y - cBall.y;
					if (difX > difY) velocitat.x = -velocitat.x;
					else if (difY > difX)  velocitat.y = -velocitat.y;
					else velocitat = -velocitat;
				}
			}
			else {
				if (oldPos.y > pos.y) velocitat.x = -velocitat.x;
				else if (oldPos.x > pos.x) velocitat.y = -velocitat.y;
				else if (topRight) velocitat = -velocitat;
				else {
					difX = posE.x+size.x - cBall.x;
					difY = posE.y+size.y - cBall.y;
					if (difX > difY) velocitat.x = -velocitat.x;
					else if (difY > difX)  velocitat.y = -velocitat.y;
					else velocitat = -velocitat;
				}
			}
		}

		switch (type) {
			case 0:
				if (elements.at(elemPos).getResistance()==0) CollisionSound->play2D("audio/brickBreak.wav");
				else CollisionSound->play2D("audio/brick.mp3");
				break;
			case 1: 
				CollisionSound->play2D("audio/coin.wav");
				break;
			case 2:
				CollisionSound->play2D("audio/alarm.mp3");
				break;
			case 3:
				//Llave
				CollisionSound->play2D("audio/key.mp3");
				break;
			case 4:
				//puerta
				break;
		}
	}
	
	return colision;
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

bool CollisionManager::collisionPolice(glm::ivec2 & pos, Player * player, glm::ivec2 & size)
{
	int xmin = pos.x;
	int xmax = pos.x + size.x;
	int ymin = pos.y;
	int ymax = pos.y + size.y;

	if (
		(player->getPosition().x + player->getSize().x > xmin) &&
		(xmax > player->getPosition().x) &&
		(player->getPosition().y + player->getSize().y > ymin-3) &&
		(ymax-3 > player->getPosition().y)
		)
	{
		return true;
	}

	return false;
}

bool CollisionManager::hayColision(glm::ivec2 & pos1, const glm::ivec2 & size1, const glm::ivec2 & pos2, const glm::ivec2 & size2) const
{
	int xmin1 = pos1.x;
	int xmax1 = pos1.x + size1.x;
	int ymin1 = pos1.y;
	int ymax1 = pos1.y + size1.y;

	int xmin2 = pos2.x;
	int xmax2 = pos2.x + size2.x;
	int ymin2 = pos2.y;
	int ymax2 = pos2.y + size2.y;

	if ((xmax2 >= xmin1) && (xmax1 >= xmin2) && (ymax2 >= ymin1) && (ymax1 >= ymin2)) return true;
	else return false;
}
