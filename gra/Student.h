#pragma once
#include "GameObject.h"


class Student : public GameObject {
   
private:
    int health;
    int maxHealth;

 
    bool isDashing;
    float dashTimer;
    float dashCooldown;
    float dashMultiplier;

   
public:

    Student(float startX, float startY);

   
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Specyficzne metody tylko dla Studenta
    void takeDamage(int damageAmount);
    int getHealth() const;
};
