// Pendulum.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>

const int windowH = 800;
const int windowW = 800;

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Pendulum");

	sf::CircleShape knot(2.5f);
	knot.setFillColor(sf::Color::Black);
	knot.setOrigin(knot.getRadius(), knot.getRadius());
	knot.setPosition(windowW / 2, windowH / 2 - 300);

	sf::CircleShape weight(10.f);
	weight.setFillColor(sf::Color::Green);
	weight.setOrigin(weight.getRadius(), weight.getRadius());
	weight.setPosition(windowW / 2, windowH / 2);
	const int weightMass = 5;
	sf::Vector2f pos = weight.getPosition();

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(knot.getPosition().x, knot.getPosition().y), sf::Color::Black),
		sf::Vertex(sf::Vector2f(weight.getPosition().x, weight.getPosition().y), sf::Color::Black)
	};
	const int lineLength = sqrt(pow(line[0].position.x - line[1].position.x, 2) +
						pow(line[0].position.y - line[1].position.y, 2));

	float theta = asin(pos.x/lineLength);
	float g = 9.81;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		

		weight.setPosition(pos);

		window.clear(sf::Color::White);
		window.draw(knot);
		window.draw(line,2,sf::Lines);
		window.draw(weight);
		window.display();
	}

	return 0;
}

