#include "Obstacle.h"

Obstacle::Obstacle(float startX, float startY, float width, float height)
    : GameObject(startX, startY, 0.0f) {

    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(105, 105, 105)); // Ciemnoszary kamień
    shape.setOrigin(width / 2.0f, height / 2.0f);
    shape.setPosition(position);
}

void Obstacle::update(float dt) {
    // Przeszkoda jest statyczna, nic nie robi w czasie
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
