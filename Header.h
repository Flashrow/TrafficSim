#pragma once
#include <sfml/Graphics.hpp>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include<vector>

const unsigned int STREET_WIDTH = 100;
const unsigned int CAR_NUMBER = 30;
const unsigned int CAR_SPEED = 2;		//pixels per single frame car move
unsigned int const FRAMERATE = 60;
unsigned int const WINDOWSIZE = 1800;
unsigned int const STREETSNUMBER = 6;
unsigned int const CAR_LENGHT = 64;
const std::string BACKGROUND_TEXTURE = "res/background.png";
const std::string CAR_TEXTURE = "res/car.png";
const std::string TRUCK_TEXTURE = "res/car1.png";

struct Hour {
	int hour;
	int minutes;
	int seconds;
};

enum class Direction{
	right, //0
	down,  //1
	left,  //2
	up     //3
};

enum class CarState {
	move,	//0
	wait,	//1
	turn,	//2
	slow,	//3
	ready	//4
};

// Struktura pozycji
struct Position {
	unsigned int posX;
	unsigned int posY;
	Direction dir;
};



// bazowa klasa obiektu graficznego
class GraphicObject {
protected: 
	Position position;
	sf::Texture objectTexture;
	sf::Sprite objectSprite;
public: 
	std::string name;
	virtual void setPosition(Position position) = 0;
	virtual Position getPosition() = 0;
	virtual sf::Sprite getDrawable() = 0;
	virtual void update(std::vector <Position> crossroads, float elapsedTime = 0.0f) = 0;
};

// klasa samochodu tymczasowa
class Car : public GraphicObject {
protected:
	float timer = 0;
	CarState state;
	Direction turningDirection;
	int turningDistance = 0;
	int rotationProgress = 0;
	Position currentCrossroad = { 0,0,Direction::right };
public:
	Position isFacingCrossroad(std::vector <Position> crossroads);
	void setPosition(Position position);
	bool outTheMap();
	Position getPosition();
	Car(Position carPosition, std::string texturePath, std::string name ="");
	sf::Sprite getDrawable();
	void turnRight();
	void turnLeft();
	void turnStraight();
	void move();
	void turn();
	void changeState(CarState state);
	CarState getState();
	void update(std::vector <Position> crossroads, float elapsedTime);
	void setCurrentCrossroad(Position crossroad);
	Position getCurrentCrossroad();
	void setCorrectPosition();
};

class Truck : public Car {
	using Car::Car;
	//Truck(Position carPosition, std::string texturePath, std::string name = "");
};


// klasa pojedynczej drogi
class StreetSection : public GraphicObject {
private:
	unsigned int streetLength;
public:
	StreetSection(Position sectionPos, unsigned int streetLength, std::string texturePath, std::string name = "");
	bool isHorizontal();
	void setPosition(Position position);
	Position getPosition();
	sf::Sprite getDrawable();
	void update(std::vector <Position> crossroads, float elapsedTime);
};

// klasa zarzadzania drogami
class StreetManager {
private:
	std::vector <GraphicObject*> streetList;
	std::vector <Position> crossroadsPositions;
	std::vector <Position> findCrossroadsPositions();
public:
	StreetManager();
	StreetManager(unsigned int screenSize, unsigned int streetsNumber, std::vector<Position>& spawnPoints);
	std::vector <Position> getCrossroads();
	void draw(sf::RenderWindow& window);
	std::vector <GraphicObject*> getStreets();
	bool isCrossing(Position position);
};

// klasa miasto
class CityManager {
private:
	float timer = 0;
	unsigned int screenSize;
	unsigned int carsNumber;
	unsigned int currentCars = 0;
	sf::Window window;
	sf::Clock clock;
	StreetManager streetManager;
	std::vector<Position> spawnPoints;
	std::vector<Car*> cars;
	std::vector<GraphicObject*> streets;
	std::vector<GraphicObject*> graphicObjects;
	sf::Sprite background;
	sf::Texture backgroundTexture;

public:
	CityManager(std::vector<Position> spawnPoints, unsigned int carsNumber, unsigned int screenSize);
	void update(sf::RenderWindow& window);
	Car* spawnCar();
	void startSim(sf::RenderWindow& window);
	void resetSim();
	void show();
	bool checkCarFront(Car* car);
	Car* findCar(Position location);
	bool rightHandRule(Car* car);
	bool checkCrossroadBusy(Position crossroad);
};





