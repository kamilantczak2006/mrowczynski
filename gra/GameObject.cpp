#include "GameObject.h"

// Konstruktor klasy bazowej
GameObject::GameObject(float startX, float startY, float startSpeed) {
    position.x = startX;
    position.y = startY;
    speed = startSpeed;
    rotationSpeed = 0.0f;
}

// Zwykłe metody (gettery)
sf::Vector2f GameObject::getPosition() const {
    return position;
}

sf::FloatRect GameObject::getBounds() const {
    return shape.getGlobalBounds();
}
