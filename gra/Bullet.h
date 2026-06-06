#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Bullet : public GameObject {
private:
    sf::Vector2f velocity;
    sf::CircleShape circleShape; // Dodajemy własne koło tylko do wyświetlania

public:
    Bullet(float startX, float startY, sf::Vector2f dir, float bulletSpeed);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};
