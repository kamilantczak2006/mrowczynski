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
        sprite.setTexture(texture);
        sprite.setOrigin(50.0f, 50.0f);
    }
}

void Boss::update(float dt) {
    if (shootTimer > 0.0f) shootTimer -= dt;

    phaseTimer -= dt;
    if (phaseTimer <= 0.0f) {
        phase = (phase + 1) % 4; // Teraz mamy 4 fazy! (0, 1, 2, 3)
        phaseTimer = 4.0f;       // Każda trwa 4 sekundy

        if (phase == 0) shootCooldown = 0.8f;      // Faza 0: Snajper
        else if (phase == 1) shootCooldown = 0.15f; // Faza 1: Karuzela
        else if (phase == 2) shootCooldown = 0.08f; // Faza 2: CKM / Wężyk (Bardzo szybko!)
        else if (phase == 3) {
            shootCooldown = 99.0f;        // Faza 3: Boss nie strzela, skupia się na magii
            hasSummonedThisPhase = false; // Resetujemy flagę przywołania dla tej fazy
        }
        sprite.setPosition(position);
    }

    if (phase == 1) sprayAngle += 3.0f * dt;
    if (phase == 2) sprinklerAngle += 8.0f * dt; // Szybkie machanie lufą na boki
}

bool Boss::tryShoot(sf::Vector2f& outDirection) {
    if (phase == 3) return false; // W fazie 3 (Nekromanta) nie strzelamy

    if (shootTimer <= 0.0f) {
        shootTimer = shootCooldown;

        if (phase == 0) {
            // Snajper
            sf::Vector2f dir = targetPosition - position;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist != 0) dir /= dist;
            outDirection = dir;
        } else if (phase == 1) {
            // Karuzela
            outDirection = sf::Vector2f(std::cos(sprayAngle), std::sin(sprayAngle));
        } else if (phase == 2) {
            // Wężyk - celuje w gracza, ale nakładamy na to funkcję sinus (falę)
            sf::Vector2f dir = targetPosition - position;
            float baseAngle = std::atan2(dir.y, dir.x);
            float offset = std::sin(sprinklerAngle) * 0.6f; // Rozrzut na boki
            float finalAngle = baseAngle + offset;
            outDirection = sf::Vector2f(std::cos(finalAngle), std::sin(finalAngle));
        }
        return true;
    }
    return false;
}

// Funkcja pytana przez główną pętlę gry
bool Boss::trySummon() {
    // Jeśli jesteśmy w fazie 3 i odczekaliśmy 1 sekundę na "naładowanie" ataku
    if (phase == 3 && phaseTimer < 3.0f && !hasSummonedThisPhase) {
        hasSummonedThisPhase = true; // Żeby nie przywoływać ich co klatkę!
        return true;
    }
    return false;
}
