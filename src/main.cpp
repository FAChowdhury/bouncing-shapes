#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace Shape
{
	class Shape
	{
	 public:
		std::string type_;
		std::string name_;
		float x_;
		float y_;
		float vx_;
		float vy_;
		int R_;
		int G_;
		int B_;

		Shape() = default;

		Shape(std::string type, std::string name, float x, float y, float vx, float vy, int R, int G, int B)
		: type_{type}
		, name_{name}
		, x_{x}
		, y_{y}
		, vx_{vx}
		, vy_{vy}
		, R_{R}
		, G_{G}
		, B_{B}
		{}

		Shape(const Shape& other)
		: type_{other.type_}
		, name_{other.name_}
		, x_{other.x_}
		, y_{other.y_}
		, vx_{other.vx_}
		, vy_{other.vy_}
		, R_{other.R_}
		, G_{other.G_}
		, B_{other.B_}
		{}

		virtual std::vector<float> getProperties() const = 0;
		virtual ~Shape() = default;
	};

	class Circle : public Shape
	{
	 public:
		float radius_;
		std::shared_ptr<sf::Shape> sf_ = nullptr;

		Circle(std::string type, std::string name, float x, float y, float vx, float vy, int R, int G, int B, float radius)
		: Shape(type, name, x, y, vx, vy, R, G, B)
		, radius_{radius}
		{}

		std::vector<float> getProperties() const override
		{
			return std::vector<float>{radius_};
		}
	};

	class Rectangle : public Shape
	{
	 public:
		float width_;
		float height_;
		std::shared_ptr<sf::Shape> sf_ = nullptr;

		Rectangle(std::string type,
		          std::string name,
		          float x,
		          float y,
		          float vx,
		          float vy,
		          int R,
		          int G,
		          int B,
		          float width,
		          float height)
		: Shape(type, name, x, y, vx, vy, R, G, B)
		, width_{width}
		, height_{height}
		{}

		std::vector<float> getProperties() const override
		{
			return std::vector<float>{width_, height_};
		}
	};
} // namespace Shape

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

	auto shapes = std::vector<std::shared_ptr<Shape::Shape>>{}; // store the shapes from configuration
	while (fin >> word)
	{
		if (word == "Window")
		{
			fin >> win_width >> win_height;
			std::cout << "window is " << win_width << " X " << win_height << std::endl;
		}
		else if (word == "Circle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> radius;
			auto circle =
			    std::make_shared<Shape::Circle>(std::string("Circle"), std::string(name), x, y, vx, vy, r, g, b, radius);
			shapes.push_back(circle);
		}
		else if (word == "Rectangle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> w >> h;
			auto rectangle =
			    std::make_shared<Shape::Rectangle>(std::string("Rectangle"), std::string(name), x, y, vx, vy, r, g, b, w, h);
			shapes.push_back(rectangle);
		}
	}

	// create sfml shapes
	auto sf_shapes = std::vector<std::shared_ptr<sf::Shape>>();
	for (const auto& shape : shapes)
	{
		if (shape->type_ == "Circle")
		{
			auto radius = shape->getProperties()[0];
			auto sfcircle = std::make_shared<sf::CircleShape>(radius);
			sfcircle->setPosition(shape->x_, shape->y_);
			sfcircle->setFillColor(sf::Color(static_cast<uint8_t>(shape->R_),
			                                 static_cast<uint8_t>(shape->G_),
			                                 static_cast<uint8_t>(shape->B_)));
			sfcircle->setOrigin(radius, radius);
			sf_shapes.push_back(sfcircle);
		}
		else
		{
			auto width = shape->getProperties()[0];
			auto height = shape->getProperties()[1];
			auto sfrectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
			sfrectangle->setPosition(shape->x_, shape->y_);
			sfrectangle->setFillColor(sf::Color(static_cast<uint8_t>(shape->R_),
			                                    static_cast<uint8_t>(shape->G_),
			                                    static_cast<uint8_t>(shape->B_)));
			sfrectangle->setOrigin(width / 2, height / 2);
			sf_shapes.push_back(sfrectangle);
		}
	}

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Bouncing Shapes");
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (std::size_t i = 0; i < sf_shapes.size(); ++i)
		{
			auto shape = shapes[i];
			auto sf_shape = sf_shapes[i];

			// if shape is hitting edge, reverse vx_ and vy_
			auto x = sf_shape->getPosition().x;
			auto y = sf_shape->getPosition().y;
			if (shape->type_ == "Circle")
			{
				if (x + shape->getProperties()[0] >= static_cast<float>(win_width))
				{
					shape->vx_ *= -1;
				}
				if (x - shape->getProperties()[0] <= 0)
				{
					shape->vx_ *= -1;
				}
				if (y + shape->getProperties()[0] >= static_cast<float>(win_height))
				{
					shape->vy_ *= -1;
				}
				if (y - shape->getProperties()[0] <= 0)
				{
					shape->vy_ *= -1;
				}
			}
			else
			{
				if (x + (shape->getProperties()[0] / 2) >= static_cast<float>(win_width))
				{
					shape->vx_ *= -1;
				}
				if (x - (shape->getProperties()[0] / 2) <= 0)
				{
					shape->vx_ *= -1;
				}
				if (y + (shape->getProperties()[1] / 2) >= static_cast<float>(win_height))
				{
					shape->vy_ *= -1;
				}
				if (y - (shape->getProperties()[1] / 2) <= 0)
				{
					shape->vy_ *= -1;
				}
			}

			sf_shape->move(shape->vx_, shape->vy_);
		}

		window.clear();

		for (const auto& shape : sf_shapes)
		{
			window.draw(*shape);
		}

		window.display();
	}

	return 0;
}