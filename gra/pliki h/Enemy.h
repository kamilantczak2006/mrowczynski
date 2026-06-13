#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
protected:
    int hp;
    sf::Vector2f targetPosition;
    sf::CircleShape circleShape;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Enemy(float startX, float startY, float startSpeed, int startHp);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void setTargetPosition(sf::Vector2f target);
    void takeDamage(int amount);
    int getHp() const;
    virtual bool trySummon();

    virtual bool tryShoot(sf::Vector2f& outDir);
};
