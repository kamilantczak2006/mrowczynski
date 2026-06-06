#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
    // Sekcja PROTECTED: Zmienne tutaj są ukryte przed resztą programu,
    // ale klasy dziedziczące (np. Student) będą miały do nich bezpośredni dostęp.
protected:
    sf::RectangleShape shape; // Wygląd obiektu
    sf::Vector2f position;    // Pozycja X, Y
    float speed;              // Prędkość w pikselach na sekundę (!)
    float rotationSpeed;      // Prędkość obrotu w stopniach na sekundę (!)

    // Sekcja PUBLIC: Metody dostępne z każdego miejsca w kodzie.
public:
    // Konstruktor
    GameObject(float startX, float startY, float startSpeed);

    // Wirtualny destruktor - niezbędny, by uniknąć wycieków pamięci przy polimorfizmie!
    virtual ~GameObject() = default;

    // --- POLIMORFIZM ---
    // Czysto wirtualne metody (oznaczone jako '= 0').
    // To zmusza każdą klasę pochodną do napisania własnej wersji tych funkcji.
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Zwykłe gettery, żeby sprawdzić, gdzie jest obiekt i czy z czymś koliduje
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};
