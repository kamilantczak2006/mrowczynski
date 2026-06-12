#include "ShooterEnemy.h"
#include <cmath>

ShooterEnemy::ShooterEnemy(float startX, float startY, float startSpeed, int startHp)
    : Enemy(startX, startY, startSpeed, startHp) {


    circleShape.setFillColor(sf::Color(255, 128, 0));
    shootCooldown = 2.0f; // Strzela co 2 sekundy
    shootTimer = shootCooldown;

    if (texture.loadFromFile("shooter.png")) {
        texture.setSmooth(false);
        sprite.setTexture(texture);

        float scaleX = shape.getSize().x / texture.getSize().x;
        float scaleY = shape.getSize().y / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    }
}

void ShooterEnemy::update(float dt) {
    Enemy::update(dt); // Ruch zostawiamy taki sam jak u zwykłego wroga

    if (shootTimer > 0.0f) {
        shootTimer -= dt;
    }
}

bool ShooterEnemy::tryShoot(sf::Vector2f& outDirection) {
    if (shootTimer <= 0.0f) {
        //wektor strzalu w strone gracza
        sf::Vector2f direction = targetPosition - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance != 0) {
            direction /= distance;
        }

        outDirection = direction;
        shootTimer = shootCooldown;
        return true;
    }
    return false;
}
