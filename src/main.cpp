#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

int main()
{
	// read in the config
	auto fin = std::ifstream("../src/config.txt"); // this is relative path from build folder
	if (fin.fail())
	{
		std::cout << "error opening file" << std::endl;
		return -1;
	}
	auto word = std::string();
	unsigned int win_width = 1920; // default width of window
	unsigned int win_height = 1080; // default height of window
	auto name = std::string();
	float x, y, vx, vy;
	int r, g, b;
	float radius;
	float w, h;

	while (fin >> word)
	{	
		if (word == "Window") {
			fin >> win_width >> win_height;
			std::cout << "window is " << win_width << " X " << win_height << std::endl;
		} else if (word == "Circle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> radius;
			std::cout << "this circle is called " << name << " and has: " << x << " " << y << " " << vx << " " << vy
			          << " " << r << " " << g << " " << b << " " << radius << std::endl;
		}
		else if (word == "Rectangle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> w >> h;
			std::cout << "this rectangle is called " << name << " and has: " << x << " " << y << " " << vx << " " << vy
			          << " " << r << " " << g << " " << b << " " << w << " " << h << std::endl;
		}
	}

	// render shapes
	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Bouncing Shapes");
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