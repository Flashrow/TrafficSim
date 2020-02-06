#include "header.h"

StreetManager::StreetManager(){
}

StreetManager::StreetManager(unsigned int screenSize, unsigned int streetsNumber, std::vector<Position>& spawnPoints) {
	streetsNumber++;
	unsigned int streetLength = screenSize;
	Position streetPos = { 0,50, Direction::right };
	while (screenSize % streetsNumber != 0) {
		screenSize--;
	}

	//horizontal streets
	for (int i = 0; i < streetsNumber-1; i++) {
		streetPos.posY += screenSize / streetsNumber;
		spawnPoints.push_back({ streetPos.posX,streetPos.posY - STREET_WIDTH/2,Direction::right});
		spawnPoints.push_back({streetPos.posX + streetLength, streetPos.posY- STREET_WIDTH/2, Direction::left});
		this->streetList.push_back(new StreetSection(streetPos,streetLength, "res/street.png"));
	}

	streetPos = { 0,0, Direction::down };
	//vertical streets
	for (int i = 0; i <= streetsNumber; i++) {
		streetPos.posX += screenSize / streetsNumber;
		spawnPoints.push_back(streetPos);
		spawnPoints.push_back({ streetPos.posX, streetPos.posY + streetLength, Direction::up });
		this->streetList.push_back(new StreetSection(streetPos, streetLength, "res/street.png"));
	}

	this->crossroadsPositions = this->findCrossroadsPositions();
}

std::vector<Position> StreetManager::getCrossroads() {
	return this->crossroadsPositions;
}

std::vector<Position> StreetManager::findCrossroadsPositions() {
	std::vector<Position> crossroads;
	for (auto coordY : this->streetList) {
		if (coordY->getPosition().dir == Direction::right
			&& coordY->getPosition().posX < WINDOWSIZE
			&& coordY->getPosition().posY < WINDOWSIZE){
			for (auto coordX : this->streetList) {
				if (coordX->getPosition().dir == Direction::down
					&& coordX->getPosition().posX < WINDOWSIZE
					&& coordX->getPosition().posY < WINDOWSIZE) {
					crossroads.push_back({
						coordX->getPosition().posX + STREET_WIDTH/2,
						coordY->getPosition().posY - STREET_WIDTH/2,
						Direction::right
						});
				}
			}
		}
	}
	return crossroads;
}



void StreetManager::draw(sf::RenderWindow& window) {
	for (auto& street : this->streetList) {
		window.draw(street->getDrawable());
	}
}

std::vector<GraphicObject*> StreetManager::getStreets(){
	return this->streetList;
}

bool StreetManager::isCrossing(Position position) {
	return false;
}
