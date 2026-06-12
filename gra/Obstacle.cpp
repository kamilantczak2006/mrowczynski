#include "Obstacle.h"

Obstacle::Obstacle(float startX, float startY, float width, float height)
    : GameObject(startX, startY, 0.0f) {

    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(width / 2.0f, height / 2.0f);
    shape.setPosition(position);

    if (texture.loadFromFile("wall.png")) {
        shape.setFillColor(sf::Color::White);
        texture.setSmooth(false);

        shape.setTexture(&texture);
    }
}

void Obstacle::update(float dt) {
}//przeszkoda statyczna ale dziedziczy po GameObject

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
