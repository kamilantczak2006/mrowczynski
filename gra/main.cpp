#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


#include "GameObject.h"
#include "Student.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalonych");
    window.setFramerateLimit(60);



    std::vector<std::unique_ptr<GameObject>> allObjects;

    
    allObjects.push_back(std::make_unique<Student>(1280.0f / 2.0f, 720.0f / 2.0f));

    sf::Clock clock;

    while (window.isOpen()) {

       
        float dt = clock.restart().asSeconds();

       
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        
        for (auto& obj : allObjects) {
            obj->update(dt);
        }

       
        window.clear(sf::Color(30, 30, 30));

        for (auto& obj : allObjects) {
            obj->draw(window);
        }

        window.display();
    }

    return 0;
}
