#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Portal : public GameObject {
private:
    sf::RectangleShape portalShape;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Portal(float startX, float startY);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};
