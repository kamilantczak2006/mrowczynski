#include "Student.h"


using namespace std;


Student::Student(float startX, float startY)
    : GameObject(startX, startY, 300.0f)
{
    hp = 3;

    isDashing = false;
    dashTimer = 0.0f;
    dashCooldown = 0.0f;
    dashMultiplier = 4.0f;


    shootCooldownTimer = 0.0f;
    damageCooldownTimer = 0.0f;


    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(25.0f, 25.0f);
    shape.setPosition(position);

    if (texture.loadFromFile("student.png")) {
        texture.setSmooth(false);

        sprite.setTexture(texture);

        // Skalowanie do hitboxa
        float scaleX = shape.getSize().x / texture.getSize().x;
        float scaleY = shape.getSize().y / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);

        // Ustawienie na srodku
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    }
}

void Student::update(float dt) {


    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (shootCooldownTimer > 0.0f) shootCooldownTimer -= dt;
    if (damageCooldownTimer > 0.0f) damageCooldownTimer -= dt;


    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0.0f) isDashing = false;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dashCooldown <= 0.0f) {
        isDashing = true;
        dashTimer = 0.2f;
        dashCooldown = 1.0f;
    }


    float currentSpeed = speed;
    if (isDashing) currentSpeed *= dashMultiplier;


    sf::Vector2f movement(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.0f;


    position += movement * currentSpeed * dt;



    if (position.x < 25) position.x = 25;
    if (position.x > 1280 - 25) position.x = 1280 - 25;
    if (position.y < 25) position.y = 25;
    if (position.y > 720 - 25) position.y = 720 - 25;


    shape.setPosition(position);
    sprite.setPosition(position);
}

void Student::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Student::takeDamage(int damageAmount) {

    if (damageCooldownTimer <= 0.0f) {
        hp -= damageAmount;
        if (hp < 0) hp = 0;
        damageCooldownTimer = 1.0f; // 1 sekunda nietykalności po trafieniu
    }
}

int Student::getHp() const {
    return hp;
}

bool Student::tryShoot(sf::Vector2f& outDirection) {
    //cooldown
    if (shootCooldownTimer > 0.0f) return false;

    bool isShooting = false;
    outDirection = sf::Vector2f(0.0f, 0.0f);


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { outDirection.y = -1.0f; isShooting = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { outDirection.y = 1.0f;  isShooting = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { outDirection.x = -1.0f; isShooting = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { outDirection.x = 1.0f;  isShooting = true; }

    if (isShooting) {
        shootCooldownTimer = 0.3f;
        return true;
    }
    return false;
}
void Student::heal(int amount) {
    hp += amount;
}
void Student::applySecondDegreeTexture() {
    if (texture.loadFromFile("student2.png")) {
        texture.setSmooth(false); //wyostrza pixele
        sprite.setTexture(texture);

        // Zabezpieczenie przed ucięciem obrazka
        sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

        // Dopasowanie do hitboxa
        float scaleX = shape.getSize().x / texture.getSize().x;
        float scaleY = shape.getSize().y / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);

        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    }
}
