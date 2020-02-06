#include "Header.h"

// sprawdza czy samochod znajdue siê przed skrzy¿owaniem
Position Car::isFacingCrossroad(std::vector<Position> crossroads) {

	switch (this->position.dir) {
	case Direction::right:
		for (auto crossroad : crossroads) {
			if (this->position.posX + CAR_LENGHT >= (crossroad.posX - STREET_WIDTH / 2 - CAR_SPEED)
				&& this->position.posX + CAR_LENGHT <= (crossroad.posX - STREET_WIDTH / 2 + CAR_SPEED)
				&& this->position.posY >= (crossroad.posY - 2)
				&& this->position.posY <= (crossroad.posY + 2)) {
				return crossroad;
			}
		}
		break;
	case Direction::down:
		for (auto crossroad : crossroads) {
			if (this->position.posY + CAR_LENGHT >= (crossroad.posY - STREET_WIDTH / 2 - CAR_SPEED)
				&& this->position.posY + CAR_LENGHT <= (crossroad.posY - STREET_WIDTH / 2 + CAR_SPEED)
				&& this->position.posX >= (crossroad.posX - 2)
				&& this->position.posX <= (crossroad.posX + 2)) {
				return crossroad;
			}
		}
		break;
	case Direction::left:
		for (auto crossroad : crossroads) {
			if (this->position.posX - CAR_LENGHT >= (crossroad.posX + STREET_WIDTH / 2 - CAR_SPEED)
				&& this->position.posX - CAR_LENGHT <= (crossroad.posX + STREET_WIDTH / 2 + CAR_SPEED)
				&& this->position.posY >= (crossroad.posY - STREET_WIDTH / 2 - 2)
				&& this->position.posY <= (crossroad.posY - STREET_WIDTH / 2 + 2)) {
				return crossroad;
			}
		}
		break;
	case Direction::up:
		for (auto crossroad : crossroads) {
			if (this->position.posY - CAR_LENGHT >= (crossroad.posY + STREET_WIDTH / 2 - CAR_SPEED)
				&& this->position.posY - CAR_LENGHT <= (crossroad.posY + STREET_WIDTH / 2 + CAR_SPEED)
				&& this->position.posX >= (crossroad.posX - 2)
				&& this->position.posX <= (crossroad.posX + 2)) {
				return crossroad;
			}
		}
		break;
	}
return Position({ 0,0,Direction::right });
}


void Car::setPosition(Position position) {
	this->position = position;
}

// sprawdza czy samochod znajduje sie poza widoczna map¹
bool Car::outTheMap() {
	if (this->position.posX >= WINDOWSIZE || this->position.posY >= WINDOWSIZE) {
		return true;
	}
	else {
		return false;
	}
}

Position Car::getPosition() {
	return this->position;
}

// tworzy samochod z parametrami - pozycja, œæie¿ka do tekstury, nazwa
Car::Car(Position carPosition, std::string texturePath, std::string name) {
	this->position = carPosition;
	this->objectTexture.loadFromFile(texturePath);
	this->name = name;
	this->state = CarState::move;

	switch (this->position.dir) {
	case Direction::right:
		
		break;
	case Direction::down:
		this->objectSprite.rotate(90);
		this->position.posX += STREET_WIDTH/2;
		break;
	case Direction::left:
		this->objectSprite.scale(-1.f, 1.f);
		this->position.posY -= STREET_WIDTH/2;
		break;
	case Direction::up:
		this->objectSprite.rotate(270);
		this->position.posX += STREET_WIDTH/2;
		break;
	}
}

// zwraca sprite samochodu
sf::Sprite Car::getDrawable() {	
	this->objectSprite.setTexture(this->objectTexture);
	this->objectSprite.setPosition(this->position.posX, this->position.posY);
	if ((int)this->position.dir == 2) {
		//std::cout << "position X:" << this->position.posX << " Y:" << this->position.posY << " direction:" << (int)this->position.dir << std::endl;
		//std::cout << "rotation:" << this->objectSprite.getRotation() << std::endl;
		//getchar();
	}
	return this->objectSprite;
}

// animacja skrêtu w prawo
void Car::turnRight() {
	switch (this->position.dir) {
	case Direction::right:
		if (this->turningDistance < CAR_LENGHT + STREET_WIDTH / 2) {
			this->position.posX += CAR_SPEED;
			this->turningDistance+= CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT + STREET_WIDTH / 2) / 2){
				this->objectSprite.rotate(360.0 / (CAR_LENGHT + STREET_WIDTH / 2));
				this->position.posY += 1;
			}
		}
		else {
			//std::cout << "rotation:" << this->objectSprite.getRotation() << std::endl;
			this->objectSprite.setRotation(90);
			this->position.dir = Direction::down;
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	case Direction::left:
		if (this->turningDistance < CAR_LENGHT) {
			this->position.posX -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT) / 2) {
				this->objectSprite.rotate(360.0 / (CAR_LENGHT));
				this->position.posY += 1;
			}
		}
		else {
			this->position.dir = Direction::up;
			this->objectSprite.setScale({ -1.0,1.0 });
			this->objectSprite.setRotation(90);
			this->objectSprite.scale({ 1.0,-1.0 });
			this->position.posX -= STREET_WIDTH / 2;
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	case Direction::down:
		if (this->turningDistance < CAR_LENGHT + (STREET_WIDTH/2)) {
			this->position.posY += CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT + (STREET_WIDTH / 2))/2) {
				this->objectSprite.rotate(360.0/ (CAR_LENGHT + (STREET_WIDTH / 2)));
				this->position.posX -= 1;
			}
		}
		else {
			this->objectSprite.setRotation(180);
			std::cout << "rotation:" << this->objectSprite.getRotation() << std::endl;
			this->objectSprite.setScale(1.0, -1.0);
			this->position.posY -= STREET_WIDTH / 2;
			this->position.dir = Direction::left;
			this->state = CarState::move;
			turningDistance = 0;
			//getchar();
		}
		break;
	case Direction::up:
		if (this->turningDistance < CAR_LENGHT + (STREET_WIDTH / 2)) {
			this->position.posY -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT + (STREET_WIDTH / 2)) / 2 + CAR_SPEED) {
				this->objectSprite.rotate(360.0 / (CAR_LENGHT + (STREET_WIDTH / 2) - CAR_SPEED));
				this->position.posX += 1;
			}
		}
		else {
			this->position.dir = Direction::right;
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	}
}

// animacja skrêtu w lewo
void Car::turnLeft() {
	switch (this->position.dir) {
	case Direction::right:
		if (this->turningDistance < CAR_LENGHT + STREET_WIDTH/2 - CAR_SPEED) {
			this->position.posX += CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT + STREET_WIDTH / 2 - CAR_SPEED) / 2) {
				this->objectSprite.rotate(-360.0 / (CAR_LENGHT + STREET_WIDTH/2 - CAR_SPEED));
				this->position.posY -= 1;
			}
		}
		else {
			this->position.dir = Direction::up;
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	case Direction::left:
		if (this->turningDistance < CAR_LENGHT + STREET_WIDTH) {
			this->position.posX -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance > (CAR_LENGHT + STREET_WIDTH) / 2) {
				this->objectSprite.rotate(-360.0 / (CAR_LENGHT + STREET_WIDTH));
				this->position.posY += 1;
			}
		}
		else {
			this->position.dir = Direction::down;
			this->position.posX += STREET_WIDTH / 2;
			this->objectSprite.scale(1.0, -1.0);
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	case Direction::down:
		if (this->turningDistance < CAR_LENGHT + STREET_WIDTH/2) {
			this->position.posY += CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance >= (CAR_LENGHT + STREET_WIDTH/2) / 2) {
				this->objectSprite.rotate(-360.0/(CAR_LENGHT + STREET_WIDTH/2));
				this->position.posX += 1;
			}
		}
		else {
			this->position.dir = Direction::right;
			//std::cout << "rotation :" << this->objectSprite.getRotation() << std::endl;

			this->objectSprite.setRotation(360);
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	case Direction::up:
		if (this->turningDistance < CAR_LENGHT + STREET_WIDTH / 2) {
			this->position.posY -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
			if (this->turningDistance >= (CAR_LENGHT + STREET_WIDTH / 2) / 2) {
				this->objectSprite.rotate(-360.0 / (CAR_LENGHT + STREET_WIDTH / 2));
				this->position.posX -= 1;
			}
		}
		else {
			this->position.dir = Direction::left;
			this->objectSprite.setRotation(180);
			this->objectSprite.setScale(1.0,-1.0);
			this->position.posY -= STREET_WIDTH / 2;
			this->state = CarState::move;
			turningDistance = 0;
		}
		break;
	}
}


// jazda prosto na skrzy¿owaniu
void Car::turnStraight() {
	switch (this->position.dir) {
	case Direction::right:
		if (this->turningDistance < STREET_WIDTH) {
			this->position.posX += CAR_SPEED;
			this->turningDistance += CAR_SPEED;
		}
		else {
			this->state = CarState::move;
			this->turningDistance = 0;
		}
		break;
	case Direction::left:
		if (this->turningDistance < STREET_WIDTH) {
			this->position.posX -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
		} else {
			this->state = CarState::move;
			this->turningDistance = 0;
		}
		break;
	case Direction::up:
		if (this->turningDistance < STREET_WIDTH) {
			this->position.posY -= CAR_SPEED;
			this->turningDistance += CAR_SPEED;
		} else {
			this->state = CarState::move;
			this->turningDistance = 0;
		}
		break;
	case Direction::down:
		if (this->turningDistance < STREET_WIDTH) {
			this->position.posY += CAR_SPEED;
			this->turningDistance += CAR_SPEED;
		} else {
			this->state = CarState::move;
			this->turningDistance = 0;
		}
		break;
	}
}

// animacja jazdy do przodu
void Car::move() {
	switch (this->position.dir) {
	case Direction::right:
		this->position.posX += CAR_SPEED;
		break;
	case Direction::down:
		this->position.posY += CAR_SPEED;
		break;
	case Direction::left:
		this->position.posX -= CAR_SPEED;
		break;
	case Direction::up:
		this->position.posY -= CAR_SPEED;
		break;
	}
}

// wybór kierunku skrêtu
void Car::turn() {
	std::srand(std::time(nullptr));
	
	int random = rand()%3;
	//std::cout << "##MAKING TURN random number:"<< random%3 << std::endl;
	switch (random % 3) {
	case 0:							// forward
		this->changeState(CarState::turn);
		this->turningDirection = Direction::up;
		break;						
	case 1:							// turn right
		this->turningDirection = Direction::right;
		this->changeState(CarState::turn);
		break;
	case 2:							// turn left
		this->turningDirection = Direction::left;
		this->changeState(CarState::turn);
		break;
	}
}

void Car::changeState(CarState state) {
	this->state = state;
}

CarState Car::getState() {
	return this->state;
}


void Car::update(std::vector <Position> crossroads, float elapsedTime) {
	//std::cout << "car" << this->name <<"moving x:" << this->position.posX << " y:" << this->position.posY << " direction:" << (int)this->position.dir << std::endl;
	switch (this->state) {
	case CarState::move:
		this->objectSprite.setColor(sf::Color::White);
		break;
	case CarState::wait:
		//std::cout << "waiting: " << this->timer << std::endl;
		if (this->timer > 0.05f) {
			this->setCorrectPosition();
			this->changeState(CarState::ready);
			timer = 0;
			//this->objectSprite.setColor(sf::Color::White);
			//this->move();
			//this->turn();
			//this->state = CarState::turn;
			//this->turningDirection = Direction::right;
		}
		else {
			timer += elapsedTime;
		}
		break;
	case CarState::turn:
		switch (this->turningDirection) {
		case Direction::up:
			this->turnStraight();
		break;
		case Direction::right:
			this->turnRight();
			if ((int)this->position.dir == 2) {
				//std::cout << "position X:" << this->position.posX << " Y:" << this->position.posY << std::endl;
				//getchar();
			}
				break;
		case Direction:: left:
			this->turnLeft();
				break;
		}
		break;
	case CarState::slow:
		break;
	case CarState::ready:
		break;
	}
	//std::cout << "elapsed time:" << this->timer << std::endl;
}

void Car::setCurrentCrossroad(Position crossroad) {
	this->currentCrossroad = crossroad;
}

Position Car::getCurrentCrossroad() {
	return this->currentCrossroad;
}

// ustawia poprawn¹ pozycjê przy skrzy¿owaniu
void Car::setCorrectPosition() {
	switch (this->position.dir) {
	case Direction::right:
		this->position.posY = this->currentCrossroad.posY;
		this->position.posX = this->currentCrossroad.posX - STREET_WIDTH / 2 - CAR_LENGHT;
		break;
	case Direction::left:
		this->position.posY = this->currentCrossroad.posY - STREET_WIDTH/2;
		this->position.posX = this->currentCrossroad.posX + STREET_WIDTH / 2 + CAR_LENGHT;
		break;
	case Direction::up:
		this->position.posY = this->currentCrossroad.posY + STREET_WIDTH / 2 + CAR_LENGHT;
		this->position.posX = this->currentCrossroad.posX;
		break;
	case Direction::down:
		this->position.posY = this->currentCrossroad.posY - STREET_WIDTH / 2 - CAR_LENGHT;
		this->position.posX = this->currentCrossroad.posX;
		break;
	}
}
