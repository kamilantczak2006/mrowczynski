#pragma once
#include "GameObject.h"

// Dziedziczenie: Student "jest" GameObjectem
class Student : public GameObject {
    // Sekcja PRIVATE: Zmienne specyficzne tylko i wyłącznie dla Studenta.
    // Nawet inne obiekty w grze nie mogą ich bezpośrednio modyfikować.
private:
    int health;
    int maxHealth;

    // Zmienne do obsługi dasha
    bool isDashing;
    float dashTimer;
    float dashCooldown;
    float dashMultiplier;

    // Sekcja PUBLIC: Interfejs Studenta
public:
    // Konstruktor Studenta
    Student(float startX, float startY);

    // Nadpisujemy (override) wirtualne metody z klasy bazowej
    // Parametr 'dt' (Delta Time) zapewni ruch niezależny od FPS
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Specyficzne metody tylko dla Studenta
    void takeDamage(int damageAmount);
    int getHealth() const;
};
