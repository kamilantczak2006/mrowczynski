#include "Portal.h"

Portal::Portal(float startX, float startY) : GameObject(startX, startY, 0.0f) {
    // Wizualny wygląd portalu (Fioletowy kwadrat, który będzie wirować)
    portalShape.setSize(sf::Vector2f(60.0f, 60.0f));
    portalShape.setFillColor(sf::Color(138, 43, 226)); // Kolor BlueViolet
    portalShape.setOrigin(30.0f, 30.0f);
    portalShape.setPosition(position);

    // Niewidzialny hitbox
    shape.setSize(sf::Vector2f(60.0f, 60.0f));
    shape.setOrigin(30.0f, 30.0f);
    shape.setPosition(position);
}

void Portal::update(float dt) {
    // Portal kręci się wokół własnej osi
    portalShape.rotate(90.0f * dt);
}

void Portal::draw(sf::RenderWindow& window) {
    window.draw(portalShape);
}
