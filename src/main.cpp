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
		std::shared_ptr<sf::Text> text_;
		float vx_;
		float vy_;
		bool draw_;

		Shape() = default;

		Shape(std::string type, std::shared_ptr<sf::Text> text, float vx, float vy, bool draw)
		: type_{type}
		, text_{text}
		, vx_{vx}
		, vy_{vy}
		, draw_{draw}
		{}

		Shape(const Shape& other)
		: text_{other.text_}
		, vx_{other.vx_}
		, vy_{other.vy_}
		, draw_{other.draw_}
		{}

		virtual std::vector<float> getProperties() const = 0;
		virtual std::shared_ptr<sf::Shape> getShape() = 0;
		virtual ~Shape() = default;
	};

	class Circle : public Shape
	{
	 public:
		std::shared_ptr<sf::CircleShape> sf_;

		Circle(std::shared_ptr<sf::Text> text,
		       float vx,
		       float vy,
		       std::shared_ptr<sf::CircleShape>& sf,
		       bool draw)
		: Shape(std::string("Circle"), text, vx, vy, draw)
		, sf_{sf}
		{}

		std::vector<float> getProperties() const override
		{
			return std::vector<float>{sf_->getRadius() * sf_->getScale().x};
		}

		std::shared_ptr<sf::Shape> getShape() override
		{
			return sf_;
		}
	};

	class Rectangle : public Shape
	{
	 public:
		std::shared_ptr<sf::RectangleShape> sf_;

		Rectangle(std::shared_ptr<sf::Text> text,
		          float vx,
		          float vy,
		          std::shared_ptr<sf::RectangleShape>& sf,
		          bool draw)
		: Shape(std::string("Rectangle"), text, vx, vy, draw)
		, sf_{sf}
		{}

		std::vector<float> getProperties() const override
		{
			return std::vector<float>{sf_->getScale().x * sf_->getSize().x, sf_->getScale().y * sf_->getSize().y};
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

	sf::Font font;
	if (!font.loadFromFile("../font/font.otf"))
	{
		std::cerr << "Error loading font\n";
		return -1;
	}

	auto names = std::vector<std::string>{};
	auto shapes = std::map<std::string, std::shared_ptr<Shape::Shape>>{};
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

			auto sfcircle = std::make_shared<sf::CircleShape>(radius);
			sfcircle->setPosition(x, y);
			sfcircle->setFillColor(sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)));
			sfcircle->setOrigin(radius, radius);

			// create text for circle
			auto text = std::make_shared<sf::Text>();
			text->setFont(font); // Set the font
			text->setString(name); // Set the text
			text->setCharacterSize(18); // Set the character size
			text->setFillColor(sf::Color::White); // Set the text color
			// Center the text on the shape
			sf::FloatRect textRect = text->getLocalBounds();
			text->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			text->setPosition(x, y);

			// make circle class
			auto circle = std::make_shared<
			    Shape::Circle>(text, vx, vy, sfcircle, true);

			shapes.insert(std::make_pair(name, circle));
			names.push_back(name);
		}
		else if (word == "Rectangle")
		{
			fin >> name >> x >> y >> vx >> vy >> r >> g >> b >> w >> h;

			auto sfrectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(w, h));
			sfrectangle->setPosition(x, y);
			sfrectangle->setFillColor(
			    sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)));
			sfrectangle->setOrigin(w / 2, h / 2);

			// create text for circle
			auto text = std::make_shared<sf::Text>();
			text->setFont(font); // Set the font
			text->setString(name); // Set the text
			text->setCharacterSize(18); // Set the character size
			text->setFillColor(sf::Color::White); // Set the text color
			// Center the text on the shape
			sf::FloatRect textRect = text->getLocalBounds();
			text->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			text->setPosition(x, y);

			// make rectangle class
			auto rectangle = std::make_shared<Shape::Rectangle>(text, vx, vy, sfrectangle, true);

			shapes.insert(std::make_pair(name, rectangle));
			names.push_back(name);
		}
	}

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Bouncing Shapes");
	bool initSuccess = ImGui::SFML::Init(window);
	if (!initSuccess)
	{
		std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
		return -1;
	}

	// Selecting Shape
	std::vector<const char*> names_cstr;
	names_cstr.reserve(names.size());
	for (const auto& name : names)
	{
		names_cstr.push_back(name.c_str());
	}
	int current_item = 0;

	// shape colour
	sf::Color shapeColor = sf::Color::Green;
	float color[3] = {shapeColor.r / 255.0f, shapeColor.g / 255.0f, shapeColor.b / 255.0f};

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

		ImGui::Begin("Bouncing Shapes!");
		ImGui::Combo("Shapes", &current_item, names_cstr.data(), static_cast<int>(names_cstr.size()));

		bool toDraw = true;
		auto it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
		if (it != shapes.end())
		{
			toDraw = it->second->draw_;
		}

		if (ImGui::Checkbox("Draw Shape", &toDraw))
		{
			auto it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
			if (it != shapes.end())
			{
				if (toDraw)
				{
					it->second->draw_ = true;
				}
				else
				{
					it->second->draw_ = false;
				}
			}
		}

		it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
		if (it != shapes.end()) 
		{
			color[0] = static_cast<float>(it->second->getShape()->getFillColor().r) / 255.0f;
			color[1] = static_cast<float>(it->second->getShape()->getFillColor().g) / 255.0f;
			color[2] = static_cast<float>(it->second->getShape()->getFillColor().b) / 255.0f;
		}

		if (ImGui::ColorEdit3("Shape Color", color))
		{
			// Update the shape color based on the selected color
			shapeColor.r = static_cast<sf::Uint8>(color[0] * 255);
			shapeColor.g = static_cast<sf::Uint8>(color[1] * 255);
			shapeColor.b = static_cast<sf::Uint8>(color[2] * 255);
			auto it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
			if (it != shapes.end())
			{
				it->second->getShape()->setFillColor(shapeColor);
			}
		}

		// shape scale
		float scale = 1.0f;
		it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)])); 
		if (it != shapes.end()) 
		{
			scale = it->second->getShape()->getScale().x;
		}

		if (ImGui::SliderFloat("Scale", &scale, 0.0f, 3.0f))
		{
			// Update the circle radius based on the slider value
			auto it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
			if (it != shapes.end())
			{
				it->second->getShape()->setScale(scale, scale);
			}
		}

		float velocity[2] = {0.5f, 0.5f};
		it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
		if (it != shapes.end())
		{
			velocity[0] = it->second->vx_;
			velocity[1] = it->second->vy_;
		}

		if (ImGui::SliderFloat2("2D Vector", velocity, -15.0f, 15.0f))
		{
			auto it = shapes.find(std::string(names_cstr[static_cast<std::size_t>(current_item)]));
			if (it != shapes.end())
			{
				it->second->vx_ = velocity[0];
				it->second->vy_ = velocity[1];
			}
		}

		ImGui::End();

		for (const auto& shape : shapes)
		{
			// if shape is hitting edge, reverse vx_ and vy_
			auto x = shape.second->getShape()->getPosition().x;
			auto y = shape.second->getShape()->getPosition().y;
			if (shape.second->type_ == "Circle")
			{
				if ((x + shape.second->getProperties()[0] >= static_cast<float>(win_width)) or (x - shape.second->getProperties()[0] <= 0))
					shape.second->vx_ *= -1;
				if ((y + shape.second->getProperties()[0] >= static_cast<float>(win_height)) or (y - shape.second->getProperties()[0] <= 0))
					shape.second->vy_ *= -1;
			}
			else
			{
				if ((x + (shape.second->getProperties()[0] / 2) >= static_cast<float>(win_width)) or (x - (shape.second->getProperties()[0] / 2) <= 0))
					shape.second->vx_ *= -1;
				if ((y + (shape.second->getProperties()[1] / 2) >= static_cast<float>(win_height)) or (y - (shape.second->getProperties()[1] / 2) <= 0))
					shape.second->vy_ *= -1;
			}

			shape.second->getShape()->move(shape.second->vx_, shape.second->vy_);
			shape.second->text_->move(shape.second->vx_, shape.second->vy_);
		}

		window.clear();

		for (const auto& shape : shapes)
		{
			if (shape.second->draw_)
			{
				window.draw(*(shape.second->getShape()));
				window.draw(*(shape.second->text_));
			}
		}
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}