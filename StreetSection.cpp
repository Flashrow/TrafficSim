#include "Header.h"

StreetSection::StreetSection(Position sectionPos, unsigned int streetLength, std::string texturePath, std::string name) {
	if (!this->objectTexture.loadFromFile(texturePath)) {
		std::cout << "Loading texture failed" << std::endl;
		std::system("pause");
	}
	this->objectTexture.setRepeated(true);
	this->position = sectionPos;
	this->streetLength = streetLength;
	this->name = name;
}

bool StreetSection::isHorizontal() {
	if (this->position.dir == Direction::right) {
		return true;
	}
	else {
		return false;
	}
}

void StreetSection::setPosition(Position position) {
	this->position = position;
}

Position StreetSection::getPosition() {
	return this->position;
}

sf::Sprite StreetSection::getDrawable(){	
	sf::Sprite streetSprite;
	streetSprite.setTexture(this->objectTexture);
	streetSprite.setTextureRect(sf::IntRect(0, 0, STREET_WIDTH, this->streetLength));
	streetSprite.setPosition(this->position.posX, this->position.posY);

	if (this->isHorizontal()) {
		streetSprite.rotate(270);
	}
	
	return streetSprite;
}

void StreetSection::update(std::vector <Position> crossroads, float elapsedTime) {
}