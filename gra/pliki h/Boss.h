#pragma once
#include "Enemy.h"

class Boss : public Enemy {
private:
    float shootTimer;
    float shootCooldown;
    int phase;
    float phaseTimer;
    float sprayAngle;

    float sprinklerAngle;
    bool hasSummonedThisPhase;

public:
    int maxHp;

    Boss(float startX, float startY, int startHp);
    void update(float dt) override;
    bool tryShoot(sf::Vector2f& outDirection) override;

    bool trySummon() override;
};
