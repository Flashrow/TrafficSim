#include "Header.h"


int main() {
	sf::RenderWindow window{ {WINDOWSIZE, WINDOWSIZE},"Title" };
	sf::Event event;
	sf::Clock clock;
	sf::Clock clockCity;

	srand((unsigned)time(0));

	window.setFramerateLimit(FRAMERATE);

	std::vector<Position> spawnPoints;
	
	CityManager cityManager(spawnPoints, CAR_NUMBER, WINDOWSIZE);
	cityManager.startSim(window);
	
	while (window.isOpen()) {
		
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed) {
				window.close();
			}
		}
		window.clear();
		sf::Time time = clock.getElapsedTime();
		std::cout << 1.0f/time.asSeconds() << std::endl;
		clock.restart().asSeconds();
		//streetManager.draw(window);
		cityManager.update(window);
		window.display();

	}
	
	return 0;
}