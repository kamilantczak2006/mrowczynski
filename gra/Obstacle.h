#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Obstacle : public GameObject {
public:
    Obstacle(float startX, float startY, float width, float height);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};
