#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Bullet : public GameObject {
private:
    sf::Vector2f velocity;
    sf::CircleShape circleShape;

public:
    bool isEnemyBullet; // NOWE

    // Zmieniony konstruktor:
    Bullet(float startX, float startY, sf::Vector2f dir, float bulletSpeed, sf::Color color, bool enemyBullet);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};
