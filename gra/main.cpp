#include <iostream>
#include <SFML/Graphics.hpp>

int main() {

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalone");
    window.setFramerateLimit(60);


    sf::RectangleShape student(sf::Vector2f(50.0f, 50.0f));
    student.setFillColor(sf::Color::Green);
    student.setPosition(1280.0f / 2.0f, 720.0f / 2.0f);


    float speed = 5.0f;


    float dashMultiplier = 4.0f;
    bool isDashing = false;
    int dashTimer = 0;
    int dashDuration = 10;
    int dashCooldown = 0;

    while (window.isOpen()) {


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (dashCooldown == 0) {
                    isDashing = true;
                    dashTimer = dashDuration;
                    dashCooldown = 60;
                }
            }
        }


        float currentSpeed = speed;


        if (isDashing) {
            currentSpeed = speed * dashMultiplier;
            dashTimer--;
            if (dashTimer <= 0) {
                isDashing = false;
            }
        }


        if (dashCooldown > 0 && !isDashing) {
            dashCooldown--;
        }


        sf::Vector2f movement(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= currentSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += currentSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= currentSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += currentSpeed;

        student.move(movement);


        sf::Vector2f pos = student.getPosition();

        if (pos.x < 0) student.setPosition(0, pos.y);
        if (pos.x > 1280 - 50) student.setPosition(1280 - 50, pos.y);


        if (pos.y < 0) student.setPosition(pos.x, 0);
        if (pos.y > 720 - 50) student.setPosition(pos.x, 720 - 50);


        window.clear(sf::Color(30, 30, 30));

        window.draw(student);

        window.display();
    }

    return 0;
}
