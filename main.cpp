#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;

int main (int argc, char *argv[]) {
	
	auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Prueba SFML 3 en ZinjaI");
	
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	
	while (window.isOpen()) {
		// En SFML 3, pollEvent() devuelve un std::optional
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		
		window.clear();
		window.draw(shape);
		window.display();
	}
	
	return 0;
}

