#include "Student.h"

// Konstruktor Studenta - najpierw wywołuje konstruktor GameObject
Student::Student(float startX, float startY)
    : GameObject(startX, startY, 300.0f) // 300.0f to nasza bazowa prędkość (piksele na sekundę)
{
    health = 100;
    maxHealth = 100;
    isDashing = false;
    dashTimer = 0.0f;
    dashCooldown = 0.0f;
    dashMultiplier = 4.0f;

    // Konfigurujemy wygląd naszego prostokąta
    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
}

void Student::update(float dt) {
    // 1. Obsługa odliczania czasu dasha (w sekundach!)
    if (dashCooldown > 0.0f) {
        dashCooldown -= dt;
    }
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0.0f) {
            isDashing = false;
        }
    }

    // 2. Aktywacja dasha (Spacja)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dashCooldown <= 0.0f) {
        isDashing = true;
        dashTimer = 0.2f;       // Dash trwa 0.2 sekundy
        dashCooldown = 1.0f;    // Musimy poczekać 1 sekundę na kolejny
    }

    // 3. Obliczanie prędkości
    float currentSpeed = speed;
    if (isDashing) {
        currentSpeed *= dashMultiplier;
    }

    // 4. Sterowanie (WASD)
    sf::Vector2f movement(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.0f;

    // 5. Aktualizacja pozycji: Dodajemy wektor kierunku przemnożony przez prędkość i czas klatki
    position += movement * currentSpeed * dt;

    // 6. Ściany
    if (position.x < 0) position.x = 0;
    if (position.x > 1280 - 50) position.x = 1280 - 50;
    if (position.y < 0) position.y = 0;
    if (position.y > 720 - 50) position.y = 720 - 50;

    // 7. Zapisanie nowej pozycji do obiektu rysującego
    shape.setPosition(position);
}

void Student::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Student::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

int Student::getHealth() const {
    return health;
}
