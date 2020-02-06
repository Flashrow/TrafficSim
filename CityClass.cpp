#include "Header.h"
#include <random>

bool compare(Position positionA, Position positionB){
		if (positionA.posX == positionB.posX
			&& positionA.posY == positionB.posY
			&& positionA.dir == positionB.dir)
			return true;
		else
			return false;
}

CityManager::CityManager(std::vector<Position> spawnPoints, unsigned int carsNumber, unsigned int screenSize) {
	this->spawnPoints = spawnPoints;
	this->carsNumber = carsNumber;
	this->screenSize = screenSize;

	this->backgroundTexture.loadFromFile(BACKGROUND_TEXTURE);
	this->background.setTexture(this->backgroundTexture);
	this->background.setTextureRect(sf::IntRect(0,0,WINDOWSIZE,WINDOWSIZE));
	this->background.setPosition({ 0,0 });
}

// update window
void CityManager::update(sf::RenderWindow &window) {
	float elapsedTime = (float)this->clock.getElapsedTime().asSeconds();
	this->timer += elapsedTime;
	std::cout << "time:" << this->timer << std::endl;
	this->clock.restart();
	if (this->timer>0.5) {
		this->timer = 0;
		//std::cout << "current cars:" << this->currentCars << " cars number:" << this->carsNumber << std::endl;
		if (this->currentCars < this->carsNumber) {
			//std::cout << "spawnowanie auta" << std::endl;
			this->cars.push_back(this->spawnCar());
			currentCars++;
		}
	}

	window.draw(this->background);

// drawing streets
	for (auto street : this->streets) {
		window.draw(street->getDrawable());
	}

// updating cars
	int objectPosition = 0;
	for (auto car : this->cars) {
		sf::RectangleShape rect({ 30,30 });
		//std::cout << "###car name:" << car->name << " state:" << (int)car->getState() << " position: X:"<<car->getPosition().posX << " Y:" << car->getPosition().posY << " direction:" <<(int) car->getPosition().dir << std::endl << "sprite rotation:" << car->getDrawable().getRotation() << " scale:(" << car->getDrawable().getScale().x << "," << car->getDrawable().getScale().y << ")" << std::endl;
		switch (car->getState()) {
		case CarState::move:

			// sprawdz czy jest samochod poprzedzaj¹cy
			if (this->checkCarFront(car)) {
				car->changeState(CarState::slow);
				break;
			}
			// sprawdzanie czy auto jest przed skrzyzowaniem
			car->setCurrentCrossroad(car->isFacingCrossroad(this->streetManager.getCrossroads()));
			if (car->getCurrentCrossroad().posX != 0) {
				car->changeState(CarState::wait);
			}
			else {
				car->setCurrentCrossroad({ 0,0,Direction::right });
				car->move();
			}
			break;
		case CarState::wait:
			
			rect.setPosition({ (float)car->getCurrentCrossroad().posX, (float)car->getCurrentCrossroad().posY });
			window.draw(rect);
			break;
		case CarState::turn:
			
			break;
		case CarState::slow:
			if (!(this->checkCarFront(car))) {
				car->changeState(CarState::move);
			}
			break;
		case CarState::ready:
			//std::cout << "ready" << std::endl;
			//getchar();
			if (!(this->checkCrossroadBusy(car->getCurrentCrossroad()))) {
				if (this->rightHandRule(car)) {
					//std::cout << "car on right" << std::endl;
					car->changeState(CarState::wait);
				}
				else {
					//std::cout << "turning now" << std::endl;
					car->turn();
				}
			}
			break;
		}
		
		sf::CircleShape circle(5);
		circle.setPosition(car->getPosition().posX, car->getPosition().posY);
		car->update(this->streetManager.getCrossroads(), elapsedTime);
		window.draw(car->getDrawable());
		window.draw(circle);


		if (car->outTheMap()) {
			this->cars.erase(this->cars.begin() + objectPosition);
			this->currentCars--;
		}
		else {
			objectPosition++;
		}
	}

	for (auto p : this->spawnPoints) {
		//std::cout << "X:" << p.posX << " Y:" << p.posY << " direction:" << (int)p.dir << std::endl;
		sf::CircleShape shape(10);
		shape.setPosition(p.posX, p.posY);
		window.draw(shape);
	}
	std::vector <Position> crossroads = this->streetManager.getCrossroads();
	for (int i = 0; i < crossroads.size(); i++) {
		sf::CircleShape shape(10);
		shape.setPosition(crossroads[i].posX, crossroads[i].posY);
		window.draw(shape);
	}
	
}

Car* CityManager::spawnCar() {
	int position = 0;
	 std::srand(std::time(nullptr));
	if (this->currentCars > 0) {
		do {
		position = rand() % this->spawnPoints.size();
      // std::cout << "position: " << std::to_string(position) <<"direction: " << (int)this->spawnPoints[position].dir << std::endl;
		} while (this->findCar(this->spawnPoints[position]) != nullptr); // TODO
	}
	else {
		position = rand() % this->spawnPoints.size();
	}

	switch ((int)rand() % 2) {
	case 0:
		return new Car(this->spawnPoints[position], CAR_TEXTURE, std::to_string(currentCars));
		break;
	case 1:
		return new Truck(this->spawnPoints[position], TRUCK_TEXTURE, std::to_string(currentCars));
		break;
	}
}

void CityManager::startSim(sf::RenderWindow& window) {
	StreetManager streetManager(WINDOWSIZE, STREETSNUMBER, this->spawnPoints);
	this->streetManager = streetManager;
	this->streets = streetManager.getStreets();

	/*int k = 0;
	for (auto i : this->spawnPoints) {
		std::cout << "SPAWN POINT #" << k << " X:" << i.posX << " Y:" << i.posY << " direction:" <<	(int) i.dir << std::endl;
		k++;
	}
	std::getchar();
	*/
}

void CityManager::resetSim() {

}

void CityManager::show() {
}

bool CityManager::checkCarFront(Car* car) {
	switch (car->getPosition().dir) {
	case Direction::right:
		if (this->findCar({ car->getPosition().posX + 3*(CAR_LENGHT / 2),
			car->getPosition().posY,
			car->getPosition().dir })) {
			return true;
		}
		break;
	case Direction::left:
		if (this->findCar({ car->getPosition().posX - 3 * (CAR_LENGHT / 2),
			car->getPosition().posY,
			car->getPosition().dir })) {
			return true;
		}
		break;
	case Direction::down:
		if (this->findCar({ car->getPosition().posX,
			car->getPosition().posY + 3 * (CAR_LENGHT / 2),
			car->getPosition().dir })) {
			return true;
		}
		break;
	case Direction::up:
		if (this->findCar({ car->getPosition().posX,
			car->getPosition().posY - 3 * (CAR_LENGHT / 2),
			car->getPosition().dir })) {
			return true;
		}
		break;
	}
	return false;
}

Car* CityManager::findCar(Position location) {
	for (auto object : this->cars) {
		//std::cout << "jest tu siema "<< typeid(*object).name()<<" a samochod to: " << typeid(Car).name()<< std::endl;
			if (object->getPosition().dir == location.dir) {
				switch (location.dir) {
				case Direction::right :
					if (object->getPosition().posX >= location.posX - CAR_LENGHT
						&& object->getPosition().posX <= location.posX
						&& object->getPosition().posY >= location.posY - STREET_WIDTH/2
						&& object->getPosition().posY <= location.posY + STREET_WIDTH/2) {
						return object;
					}
					break;
				case Direction::down:
					if (object->getPosition().posY >= location.posY - CAR_LENGHT
						&& object->getPosition().posY <= location.posY
						&& object->getPosition().posX >= location.posX - STREET_WIDTH / 2
						&& object->getPosition().posX <= location.posX + STREET_WIDTH / 2) {
						return object;
					}
					break;
				case Direction::left:
					if (object->getPosition().posX <= location.posX + CAR_LENGHT
						&& object->getPosition().posX >= location.posX
						&& object->getPosition().posY >= location.posY - STREET_WIDTH / 2
						&& object->getPosition().posY <= location.posY + STREET_WIDTH / 2) {
						return object;
					}
					break;
				case Direction::up:
					if (object->getPosition().posY <= location.posY + CAR_LENGHT
						&& object->getPosition().posY >= location.posY
						&& object->getPosition().posX >= location.posX - STREET_WIDTH / 2
						&& object->getPosition().posX <= location.posX + STREET_WIDTH / 2) {
						return object;
					}
					break;
				}
			}
	}
	return nullptr;
}

bool CityManager::rightHandRule(Car* car) {
	switch (car->getPosition().dir) {
	case Direction::right:
		if (this->findCar({ car->getCurrentCrossroad().posX + STREET_WIDTH / 4,
			car->getCurrentCrossroad().posY + STREET_WIDTH,
			Direction::up }))
			return true;
			break;
	case Direction::left:
		if (this->findCar({ car->getCurrentCrossroad().posX - STREET_WIDTH / 4,
			car->getCurrentCrossroad().posY - STREET_WIDTH,
			Direction::down }))
			return true;
		break;
	case Direction::up:
		if (this->findCar({ car->getCurrentCrossroad().posX + STREET_WIDTH,
			car->getCurrentCrossroad().posY - STREET_WIDTH/4,
			Direction::left }))
			return true;
		break;
	case Direction::down:
		if (this->findCar({ car->getCurrentCrossroad().posX - STREET_WIDTH,
			car->getCurrentCrossroad().posY + STREET_WIDTH / 4,
			Direction::right }))
			return true;
		break;
	}
	return false;
}

bool CityManager::checkCrossroadBusy(Position crossroad) {
	for (auto car : this->cars) {
		if (car->getPosition().posX > (crossroad.posX - STREET_WIDTH / 2 - CAR_LENGHT)
			&& car->getPosition().posX < (crossroad.posX + STREET_WIDTH / 2 + CAR_LENGHT)
			&& car->getPosition().posY > (crossroad.posY - STREET_WIDTH / 2 - CAR_LENGHT)
			&& car->getPosition().posY < (crossroad.posY + STREET_WIDTH / 2 + CAR_LENGHT)) {
			return true;
		}
	}
return false;
}
