#pragma once
#include "Enemy.h"

class ShooterEnemy : public Enemy {
private:
    float shootTimer;
    float shootCooldown;

public:
    ShooterEnemy(float startX, float startY, float startSpeed, int startHp);
    void update(float dt) override;
    bool tryShoot(sf::Vector2f& outDirection) override;
};
