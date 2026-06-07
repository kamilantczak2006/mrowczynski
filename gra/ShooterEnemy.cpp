#include "ShooterEnemy.h"
#include <cmath>

ShooterEnemy::ShooterEnemy(float startX, float startY, float startSpeed, int startHp)
    : Enemy(startX, startY, startSpeed, startHp) { // Wywołujemy konstruktor rodzica

    // Zmieniamy kolor na pomarańczowy, żeby gracz wiedział, kto do niego wali
    circleShape.setFillColor(sf::Color(255, 128, 0));
    shootCooldown = 2.0f; // Strzela co 2 sekundy
    shootTimer = shootCooldown;
}

void ShooterEnemy::update(float dt) {
    Enemy::update(dt); // Ruch zostawiamy taki sam jak u zwykłego wroga

    if (shootTimer > 0.0f) {
        shootTimer -= dt;
    }
}

bool ShooterEnemy::tryShoot(sf::Vector2f& outDirection) {
    if (shootTimer <= 0.0f) {
        // Obliczamy wektor strzału idealnie w stronę gracza
        sf::Vector2f direction = targetPosition - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance != 0) {
            direction /= distance;
        }

        outDirection = direction;
        shootTimer = shootCooldown; // Resetujemy licznik
        return true;
    }
    return false;
}
