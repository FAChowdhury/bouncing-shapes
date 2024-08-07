#include "imgui-SFML.h"
#include "imgui.h"
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
		virtual std::shared_ptr<sf::Shape> getShape() = 0;
		virtual ~Shape() = default;
	};

	class Circle : public Shape
	{
	 public:
		float radius_;
		std::shared_ptr<sf::CircleShape> sf_;

		Circle(std::string type, std::string name, float x, float y, float vx, float vy, int R, int G, int B, float radius)
		: Shape(type, name, x, y, vx, vy, R, G, B)
		, radius_{radius}
		{}

		std::vector<float> getProperties() const override
		{
			return std::vector<float>{radius_};
		}

		std::shared_ptr<sf::Shape> getShape() override
		{
			return sf_;
		}
	};

	class Rectangle : public Shape
	{
	 public:
		float width_;
		float height_;
		std::shared_ptr<sf::RectangleShape> sf_;

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

		std::shared_ptr<sf::Shape> getShape() override
		{
			return sf_;
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
			auto radius = circle->getProperties()[0];
			auto sfcircle = std::make_shared<sf::CircleShape>(radius);
			sfcircle->setPosition(circle->x_, circle->y_);
			sfcircle->setFillColor(sf::Color(static_cast<uint8_t>(circle->R_),
			                                 static_cast<uint8_t>(circle->G_),
			                                 static_cast<uint8_t>(circle->B_)));
			sfcircle->setOrigin(radius, radius);
			circle->sf_ = sfcircle;
			shapes.push_back(circle);
		}
		else if (word == "Rectangle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> w >> h;
			auto rectangle =
			    std::make_shared<Shape::Rectangle>(std::string("Rectangle"), std::string(name), x, y, vx, vy, r, g, b, w, h);
			auto width = rectangle->getProperties()[0];
			auto height = rectangle->getProperties()[1];
			auto sfrectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
			sfrectangle->setPosition(rectangle->x_, rectangle->y_);
			sfrectangle->setFillColor(sf::Color(static_cast<uint8_t>(rectangle->R_),
			                                    static_cast<uint8_t>(rectangle->G_),
			                                    static_cast<uint8_t>(rectangle->B_)));
			sfrectangle->setOrigin(width / 2, height / 2);
			rectangle->sf_ = sfrectangle;
			shapes.push_back(rectangle);
		}
	}

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Bouncing Shapes");
	bool initSuccess = ImGui::SFML::Init(window);
	if (!initSuccess)
	{
		std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
		return -1;
	}

	window.setFramerateLimit(60);
	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		for (const auto& shape : shapes)
		{
			// if shape is hitting edge, reverse vx_ and vy_
			auto x = shape->getShape()->getPosition().x;
			auto y = shape->getShape()->getPosition().y;
			if (shape->type_ == "Circle")
			{
				if (x + shape->getProperties()[0] >= static_cast<float>(win_width))
					shape->vx_ *= -1;
				if (x - shape->getProperties()[0] <= 0)
					shape->vx_ *= -1;
				if (y + shape->getProperties()[0] >= static_cast<float>(win_height))
					shape->vy_ *= -1;
				if (y - shape->getProperties()[0] <= 0)
					shape->vy_ *= -1;
			}
			else
			{
				if (x + (shape->getProperties()[0] / 2) >= static_cast<float>(win_width))
					shape->vx_ *= -1;
				if (x - (shape->getProperties()[0] / 2) <= 0)
					shape->vx_ *= -1;
				if (y + (shape->getProperties()[1] / 2) >= static_cast<float>(win_height))
					shape->vy_ *= -1;
				if (y - (shape->getProperties()[1] / 2) <= 0)
					shape->vy_ *= -1;
			}

			shape->getShape()->move(shape->vx_, shape->vy_);
		}

		// ImGui UI Start
		ImGui::Begin("Bouncing Shapes");
		ImGui::Text("Window text!");
		ImGui::End();
		// ImGui UI End

		window.clear();

		for (const auto& shape : shapes)
		{
			window.draw(*(shape->getShape()));
		}
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}