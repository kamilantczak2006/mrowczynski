#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Nasze nowe nagłówki
#include "GameObject.h"
#include "Student.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalonych");
    window.setFramerateLimit(60);



    std::vector<std::unique_ptr<GameObject>> allObjects;

    // Tworzymy naszego Studenta i dorzucamy go do wektora.
    // std::make_unique to najbezpieczniejszy, nowoczesny sposób na alokację pamięci (wymóg ze wskaźnikami).
    allObjects.push_back(std::make_unique<Student>(1280.0f / 2.0f, 720.0f / 2.0f));

    // Zegar do obliczania Delta Time (czasu między klatkami, kluczowe do ruchu w pikselach/sekundę)
    sf::Clock clock;

    while (window.isOpen()) {

        // Zapisujemy czas od ostatniej klatki i od razu resetujemy stoper
        float dt = clock.restart().asSeconds();

        // ETAP A: Input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // ETAP B: Logika (Update)
        // POLIMORFIZM W AKCJI: Pętla przechodzi po wszystkich obiektach.
        // Kompilator w locie sam decyduje, czy wywołać update() dla Studenta, czy dla Pułapki.
        for (auto& obj : allObjects) {
            obj->update(dt);
        }

        // ETAP C: Rysowanie
        window.clear(sf::Color(30, 30, 30));

        // Dokładnie to samo z rysowaniem - wszystko wywołuje się z jednego miejsca!
        for (auto& obj : allObjects) {
            obj->draw(window);
        }

        window.display();
    }

    return 0;
}
