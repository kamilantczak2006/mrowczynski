#include "Boss.h"
#include <cmath>

Boss::Boss(float startX, float startY, int startHp)
    : Enemy(startX, startY, 0.0f, startHp) {

    maxHp = startHp;

    circleShape.setRadius(50.0f);
    circleShape.setFillColor(sf::Color(139, 0, 0));
    circleShape.setOrigin(50.0f, 50.0f);
    circleShape.setPosition(position);

    shape.setSize(sf::Vector2f(100.0f, 100.0f));
    shape.setOrigin(50.0f, 50.0f);
    shape.setPosition(position);

    shootCooldown = 1.0f;
    shootTimer = shootCooldown;
    phase = 0;
    phaseTimer = 4.0f;
    sprayAngle = 0.0f;
    sprinklerAngle = 0.0f;
    hasSummonedThisPhase = false;
    if (texture.loadFromFile("boss.png")) {
        texture.setSmooth(false);
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

        float scaleX = shape.getSize().x / texture.getSize().x;
        float scaleY = shape.getSize().y / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setPosition(position);
    }
}

void Boss::update(float dt) {
    if (shootTimer > 0.0f) shootTimer -= dt;

    phaseTimer -= dt;
    if (phaseTimer <= 0.0f) {
        phase = (phase + 1) % 4;
        phaseTimer = 4.0f;
        if (phase == 0) shootCooldown = 0.8f;
        else if (phase == 1) shootCooldown = 0.15f; //Karuzela strzałów
        else if (phase == 2) shootCooldown = 0.08f; //rozbrysk
        else if (phase == 3) {
            shootCooldown = 99.0f;        // spawn sług
            hasSummonedThisPhase = false;
        }
        sprite.setPosition(position);
    }

    if (phase == 1) sprayAngle += 3.0f * dt;
    if (phase == 2) sprinklerAngle += 8.0f * dt;
}

bool Boss::tryShoot(sf::Vector2f& outDirection) {
    if (phase == 3) return false;

    if (shootTimer <= 0.0f) {
        shootTimer = shootCooldown;

        if (phase == 0) {
            // zwykły
            sf::Vector2f dir = targetPosition - position;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist != 0) dir /= dist;
            outDirection = dir;
        } else if (phase == 1) {
            // Karuzela
            outDirection = sf::Vector2f(std::cos(sprayAngle), std::sin(sprayAngle));
        } else if (phase == 2) {

            sf::Vector2f dir = targetPosition - position;
            float baseAngle = std::atan2(dir.y, dir.x);
            float offset = std::sin(sprinklerAngle) * 0.6f; // rozbrysk
            float finalAngle = baseAngle + offset;
            outDirection = sf::Vector2f(std::cos(finalAngle), std::sin(finalAngle));
        }
        return true;
    }
    return false;
}


bool Boss::trySummon() {
    if (phase == 3 && phaseTimer < 3.0f && !hasSummonedThisPhase) {
        hasSummonedThisPhase = true;
        return true;
    }
    return false;
}
