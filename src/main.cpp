#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

int main()
{
	// read in the config
	auto fin = std::ifstream("../src/config.txt"); // this is relative path from build folder
	if (fin.fail()) {
		std::cout << "error opening file" << std::endl;
		return -1;
	}

	// read window properties
	std::string word;
	fin >> word;
	unsigned int win_width = 100;
	unsigned int win_height = 100;
	fin >> win_width;
	fin >> win_height;

	// read shapes
	while (fin >> word) {
		if (word == "Circle") {
			std::cout << "It's a circle" << std::endl;
			break;
		} else if (word == "Rectangle") {
			std::cout << "It's a Rectangle" << std::endl;
			break;
		}
	}

	// render shapes
	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}