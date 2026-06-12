#include "Enemy.h"
#include <cmath>

using namespace std;

Enemy::Enemy(float startX, float startY, float startSpeed, int startHp)
    : GameObject(startX, startY, startSpeed) {

    hp = startHp;


    circleShape.setRadius(20.0f);
    circleShape.setFillColor(sf::Color::Red);
    circleShape.setOrigin(20.0f, 20.0f);
    circleShape.setPosition(position);


    shape.setSize(sf::Vector2f(40.0f, 40.0f));
    shape.setOrigin(20.0f, 20.0f);
    if (texture.loadFromFile("enemy.png")) {
        sprite.setTexture(texture);
        sprite.setOrigin(25.0f, 25.0f);
    }
}

void Enemy::setTargetPosition(sf::Vector2f target) {
    targetPosition = target;
}

void Enemy::update(float dt) {

    sf::Vector2f direction = targetPosition - position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction /= distance;
    }


   position += direction * speed * dt;


    circleShape.setPosition(position);
    shape.setPosition(position);
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
}

int Enemy::getHp() const {
    return hp;
}
bool Enemy::tryShoot(sf::Vector2f& outDir) {
    return false;
}
bool Enemy::trySummon() {
    return false;
}
