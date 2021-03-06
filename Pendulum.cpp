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

	sf::CircleShape weight1(15.f);
	weight1.setFillColor(sf::Color::Green);
	weight1.setOrigin(weight1.getRadius(), weight1.getRadius());
	weight1.setPosition(windowW / 2, windowH / 2);
	const int m1 = 10;
	sf::Vector2f pos1 = weight1.getPosition();

	sf::CircleShape weight2(15.f);
	weight2.setFillColor(sf::Color::Green);
	weight2.setOrigin(weight2.getRadius(), weight2.getRadius());
	weight2.setPosition(pos1.x, pos1.y + 200);
	const int m2 = 20;
	sf::Vector2f pos2 = weight2.getPosition();

	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(knot.getPosition().x, knot.getPosition().y), sf::Color::Black),
		sf::Vertex(sf::Vector2f(weight1.getPosition().x, weight1.getPosition().y), sf::Color::Black)
	};
	const int lineLength1 = sqrt(pow(line1[0].position.x - line1[1].position.x, 2) +
						pow(line1[0].position.y - line1[1].position.y, 2));

	sf::Vertex line2[] =
	{
		sf::Vertex(sf::Vector2f(weight1.getPosition().x, weight1.getPosition().y), sf::Color::Black),
		sf::Vertex(sf::Vector2f(weight2.getPosition().x, weight2.getPosition().y), sf::Color::Black)
	};
	const int lineLength2 = sqrt(pow(line2[0].position.x - line2[1].position.x, 2) +
		pow(line2[0].position.y - line2[1].position.y, 2));

	float thetaMax1 = 45; //initial conditions
	float theta1 = thetaMax1;
	float omega1 = 0; //angular speed
	float epsilon1 = 0; //angular acceleration
	bool isMoving1 = false;

	float thetaMax2 = 45; //initial conditions
	float theta2 = thetaMax2;
	float omega2 = 0; //angular speed
	float epsilon2 = 0; //angular acceleration
	bool isMoving2 = false;

	float g = 9.81f;
	float dt;

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;
	sf::Time timePerFrame = sf::seconds(1.0f / 60);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::MouseButtonPressed)
				if(event.mouseButton.button == sf::Mouse::Left)
					if (weight1.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
					{
						isMoving1 = true;
					}

			if (event.type == sf::Event::MouseMoved && isMoving1)
			{
				float x = sf::Mouse::getPosition(window).x;

				if (x >= (knot.getPosition().x + lineLength1)) x = knot.getPosition().x + lineLength1;
				else if (x <= (knot.getPosition().x - lineLength1)) x = knot.getPosition().x - lineLength1;

				if (x > knot.getPosition().x) x -= knot.getPosition().x;
				else if (x < knot.getPosition().x) x -= knot.getPosition().x;

				theta1 = asin(x / lineLength1);
				epsilon1 = 0;
				omega1 = 0;
			}

			if (event.type == sf::Event::MouseButtonReleased)
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					isMoving1 = false;
				}
		}

		timeSinceLastUpdate = clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			dt = timePerFrame.asSeconds()*10;
		}
		dt = timeSinceLastUpdate.asSeconds()*10;

		if (!isMoving1)
		{
			float top11 = -g * (2 * m1 + m2)*sin(theta1) - m2 * g*sin(theta1 - 2 * theta2);
			float top12 = -2 * sin(theta1 - theta2)*m2*(pow(omega2, 2)*lineLength2 + pow(omega1, 2)*lineLength1*cos(theta1 - theta2));
			float bot1 = lineLength1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2));

			float top21 = g * (m1 + m2)*cos(theta1) + pow(omega2, 2)*lineLength2*m2*cos(theta1 - theta2);
			float top22 = 2 * sin(theta1 - theta2)*(pow(omega1, 2)*lineLength1*(m1 + m2) + top21);
			float bot2 = lineLength2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2));

			epsilon1 = (top11 + top12) / bot1;
			omega1 += epsilon1 * dt;
			theta1 += omega1 * dt;

			epsilon2 = top22 / bot2;
			omega2 += epsilon2 * dt;
			theta2 += omega2 * dt;
		}
		
		pos1.x = sin(theta1)*lineLength1 + knot.getPosition().x;
		pos1.y = cos(theta1)*lineLength1 + knot.getPosition().y;
		weight1.setPosition(pos1);

		pos2.x = pos1.x + sin(theta2)*lineLength2;
		pos2.y = pos1.y + cos(theta2)*lineLength2;
		weight2.setPosition(pos2);

		line1[1].position = pos1;
		line2[0].position = pos1;
		line2[1].position = pos2;

		window.clear(sf::Color::White);
		window.draw(line1,2,sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.draw(knot);
		window.draw(weight1);
		window.draw(weight2);
		window.display();
	}

	return 0;
}

