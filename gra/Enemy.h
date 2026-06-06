#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Enemy : public GameObject {
private:
    int hp;
    sf::Vector2f targetPosition; // Zmienna pamiętająca, gdzie jest gracz
    sf::CircleShape circleShape; // Wizualne koło

public:
    Enemy(float startX, float startY, float startSpeed, int startHp);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Metody do obsługi logiki wroga
    void setTargetPosition(sf::Vector2f target);
    void takeDamage(int amount);
    int getHp() const;
};
