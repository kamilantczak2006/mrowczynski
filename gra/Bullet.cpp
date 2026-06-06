#include "Bullet.h"

using namespace std;

Bullet::Bullet(float startX, float startY, sf::Vector2f dir, float bulletSpeed)
    : GameObject(startX, startY, bulletSpeed) {

    velocity = dir * speed;


    circleShape.setRadius(6.0f);
    circleShape.setFillColor(sf::Color::Cyan);
    circleShape.setOrigin(6.0f, 6.0f);
    circleShape.setPosition(position);


    shape.setSize(sf::Vector2f(12.0f, 12.0f));
    shape.setOrigin(6.0f, 6.0f);

}

void Bullet::update(float dt) {
    position += velocity * dt;


    circleShape.setPosition(position);
    shape.setPosition(position);
}

void Bullet::draw(sf::RenderWindow& window) {

    window.draw(circleShape);
}
