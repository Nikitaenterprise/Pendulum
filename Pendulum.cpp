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

	sf::CircleShape weight(15.f);
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

	float thetaMax = 45; //initial conditions
	float theta = thetaMax;
	float omega = 0; //angular speed
	float epsilon = 0; //angular acceleration
	float g = 9.81;
	float dt = 0.002;

	sf::Clock clock;

	bool isMoving = false;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::MouseButtonPressed)
				if(event.mouseButton.button == sf::Mouse::Left)
					if (weight.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
					{
						isMoving = true;
						auto alpha = asin(sf::Mouse::getPosition(window).x / lineLength);
						auto beta = asin(sf::Mouse::getPosition(window).y / lineLength);
						theta = 180 - alpha - beta;
						std::cout << alpha << "\t" << beta << "\t" << theta << std::endl;
					}

			if (event.type == sf::Event::MouseButtonReleased)
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					isMoving = false;
				}
		}

		if (!isMoving)
		{
			//theta = thetaMax * sin(sqrt(g / lineLength)*clock.getElapsedTime().asSeconds());
			epsilon = -g / lineLength * sin(theta);
			omega += epsilon * dt;
			theta += omega * dt;
			//std::cout << epsilon << "\t" << omega << "\t" << theta << std::endl;
		}
		/*else if (isMoving)
		{
			theta = theta
		}*/
		pos.x = sin(theta)*lineLength + knot.getPosition().x;
		pos.y = cos(theta)*lineLength + knot.getPosition().y;
		weight.setPosition(pos);

		line[1].position = pos;

		window.clear(sf::Color::White);
		window.draw(line,2,sf::Lines);
		window.draw(knot);
		window.draw(weight);
		window.display();
	}

	return 0;
}

