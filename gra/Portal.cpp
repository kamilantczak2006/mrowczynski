#include "Portal.h"

Portal::Portal(float startX, float startY) : GameObject(startX, startY, 0.0f) {
    //hitbox
    shape.setSize(sf::Vector2f(60.0f, 60.0f));
    shape.setOrigin(30.0f, 30.0f);
    shape.setPosition(position);

    //ładowanie grafiki
    if (texture.loadFromFile("Portal.png")) {
        texture.setSmooth(false);
        sprite.setTexture(texture);

        // Zabezpieczenie rozmiaru
        sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

        // Skalowanie
        float scaleX = shape.getSize().x / texture.getSize().x;
        float scaleY = shape.getSize().y / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);

        // Wyśrodkowanie punktu obrotu
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setPosition(position);
    }
}

void Portal::update(float dt) {
    sprite.rotate(90.0f * dt);
}

void Portal::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
