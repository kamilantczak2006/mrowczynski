#include "GameObject.h"


GameObject::GameObject(float startX, float startY, float startSpeed) {
    position.x = startX;
    position.y = startY;
    speed = startSpeed;
    rotationSpeed = 0.0f;
}

void GameObject::forceMove(float dx, float dy) {
    position.x += dx;
    position.y += dy;
    shape.setPosition(position);
}

sf::Vector2f GameObject::getPosition() const {
    return position;
}

sf::FloatRect GameObject::getBounds() const {
    return shape.getGlobalBounds();
}
