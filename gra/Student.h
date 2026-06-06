#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>


class Student : public GameObject {

private:
    int hp;



    bool isDashing;
    float dashTimer;
    float dashCooldown;
    float dashMultiplier;



    float shootCooldownTimer;
    float damageCooldownTimer;

public:
    Student(float startX, float startY);


    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;


    void takeDamage(int damageAmount);
    int getHp() const;


    bool tryShoot(sf::Vector2f& outDirection);
};
