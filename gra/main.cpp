#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Bullet{
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

struct Enemy{
    sf::CircleShape shape;
    float speed;
    int hp;
};


int main() {

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalonych");
    window.setFramerateLimit(60);

    sf::Font font;
    bool hasFont = font.loadFromFile("arial.ttf");
    if (!hasFont) {
        std::cout << "UWAGA: Nie znaleziono pliku arial.ttf. UI bedzie wyswietlane w konsoli!" << std::endl;
    }

    sf::Text uiText;
    if (hasFont) {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        uiText.setPosition(20.0f, 20.0f);
    }


    sf::RectangleShape player(sf::Vector2f(50.0f, 50.0f));
    player.setFillColor(sf::Color::Green);
    player.setOrigin(25.0f, 25.0f);
    player.setPosition(1280.0f / 2.0f, 720.0f / 2.0f);


    float speed = 5.0f;
    int playerHp = 3;
    sf::Clock damageClock;
    float damageCooldown = 1.0f;

    float dashMultiplier = 4.0f;
    bool isDashing = false;
    int dashTimer = 0;
    int dashDuration = 10;
    int dashCooldown = 0;

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;


    sf::Clock shootClock;
    float shootCooldown = 0.3f;

    int currentLevel = 1;
    int enemiesToSpawnThisLevel = 3;
    int enemiesSpawnedSoFar = 0;

    sf::Clock enemySpawnClock;
    float enemySpawnCooldown = 1.5f;

    bool isGameWon = false;
    bool isGameOver = false;

    while (window.isOpen()) {


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if(!isGameWon && !isGameOver){
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    if (dashCooldown == 0) {
                        isDashing = true;
                        dashTimer = dashDuration;
                        dashCooldown = 60;
                    }
                }
            }
        }
        if(!isGameWon && !isGameOver){
            if (playerHp <= 0) {
                std::cout << "GAME OVER! Przegrales na poziomie " << currentLevel << std::endl;
                isGameOver = true;
                if (hasFont) {
                    uiText.setString("GAME OVER!\nPrzegrales na poziomie: " + std::to_string(currentLevel) +
                                     "\n\nZamknij okno, aby wyjsc.");
                }
            }

            if (enemiesSpawnedSoFar >= enemiesToSpawnThisLevel && enemies.empty()) {
                currentLevel++;
                enemiesSpawnedSoFar = 0;

                if (currentLevel == 2) {
                    enemiesToSpawnThisLevel = 5;
                    std::cout << "POZIOM 2 ROZPOCZETY! (5 przeciwnikow)" << std::endl;
                } else if (currentLevel == 3) {
                    enemiesToSpawnThisLevel = 8;
                    std::cout << "POZIOM 3 ROZPOCZETY! (8 przeciwnikow)" << std::endl;
                }
                // else if (currentLevel == 4) {
                //     enemiesToSpawnThisLevel = 10;
                //     std::cout << "POZIOM 4 ROZPOCZETY! (10 przeciwnikow)" << std::endl;
                // }
                // else if (currentLevel == 5) {
                //     enemiesToSpawnThisLevel = 15;
                //     std::cout << "POZIOM 5 ROZPOCZETY! (15 przeciwnikow)" << std::endl;
                // }
                else {
                    std::cout << "GRATULACJE! UKONCZYLES CALA GRE!" << std::endl;
                    isGameWon = true;
                    if (hasFont) {
                        uiText.setCharacterSize(36);
                        uiText.setPosition(1280.0f / 4.0f, 720.0f / 3.0f);
                        uiText.setString("GRATULACJE! UKONCZYLES CALA GRE!\n\n"
                                         "KONCOWY POZIOM: " + std::to_string(currentLevel - 1) +
                                         "\nPOZOSTALE HP: " + std::to_string(playerHp) +
                                         "\n\nZamknij okno, aby wyjsc.");
                    }
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                    }
                }
            }
        }
        if (!isGameWon && !isGameOver) {
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

            player.move(movement);


            sf::Vector2f pos = player.getPosition();

            if (pos.x < 25) player.setPosition(25, pos.y);
            if (pos.x > 1280 - 25) player.setPosition(1280 - 25, pos.y);


            if (pos.y < 25) player.setPosition(pos.x, 25);
            if (pos.y > 720 - 25) player.setPosition(pos.x, 720 - 25);

            if (enemiesSpawnedSoFar < enemiesToSpawnThisLevel) {
                if (enemySpawnClock.getElapsedTime().asSeconds() >= enemySpawnCooldown) {
                    Enemy newEnemy;
                    newEnemy.shape.setRadius(20.0f);
                    newEnemy.shape.setFillColor(sf::Color::Red);
                    newEnemy.shape.setOrigin(20.0f, 20.0f);

                    // Przeciwnicy delikatnie przyspieszają z poziomu na poziom
                    newEnemy.speed = 2.0f + (currentLevel * 0.4f);
                    newEnemy.hp = 2;

                    int edge = rand() % 4;
                    if (edge == 0)      newEnemy.shape.setPosition(rand() % 1280, -20);
                    else if (edge == 1) newEnemy.shape.setPosition(rand() % 1280, 740);
                    else if (edge == 2) newEnemy.shape.setPosition(-20, rand() % 720);
                    else                newEnemy.shape.setPosition(1300, rand() % 720);

                    enemies.push_back(newEnemy);
                    enemiesSpawnedSoFar++;
                    enemySpawnClock.restart();
                }
            }

            if (shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
                sf::Vector2f shootDir(0.0f, 0.0f);
                bool isShooting = false;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { shootDir.y = -1.0f; isShooting = true; }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { shootDir.y = 1.0f;  isShooting = true; }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { shootDir.x = -1.0f; isShooting = true; }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { shootDir.x = 1.0f;  isShooting = true; }

                if (isShooting) {
                    Bullet newBullet;
                    newBullet.shape.setRadius(6.0f);
                    newBullet.shape.setFillColor(sf::Color::Cyan);

                    newBullet.shape.setOrigin(6.0f, 6.0f);

                    newBullet.shape.setPosition(player.getPosition());

                    float bulletSpeed = 7.0f;
                    newBullet.velocity = shootDir * bulletSpeed;

                    bullets.push_back(newBullet);
                    shootClock.restart();
                }
            }

            for (size_t i = 0; i < bullets.size(); ) {
                bullets[i].shape.move(bullets[i].velocity);

                sf::Vector2f bPos = bullets[i].shape.getPosition();

                if (bPos.x < 0 || bPos.x > 1280 || bPos.y < 0 || bPos.y > 720) {
                    bullets.erase(bullets.begin() + i);
                } else {
                    i++;
                }
            }

            for (size_t i = 0; i < enemies.size(); ) {
                sf::Vector2f enemyPos = enemies[i].shape.getPosition();
                sf::Vector2f playerPos = player.getPosition();

                sf::Vector2f direction = playerPos - enemyPos;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                if (distance != 0) {
                    direction /= distance;
                }
                enemies[i].shape.move(direction * enemies[i].speed);

                if (enemies[i].shape.getGlobalBounds().intersects(player.getGlobalBounds())) {
                    if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
                        playerHp--;
                        damageClock.restart();
                        if (!hasFont) {
                            std::cout << "TRAFIENIE! HP: " << playerHp << " | Poziom: " << currentLevel << std::endl;
                        }
                    }
                }

                bool enemyKilled = false;
                for (size_t j = 0; j < bullets.size(); ) {
                    if (enemies[i].shape.getGlobalBounds().intersects(bullets[j].shape.getGlobalBounds())) {
                        enemies[i].hp--;
                        bullets.erase(bullets.begin() + j);

                        if (enemies[i].hp <= 0) {
                            enemyKilled = true;
                            break;
                        }
                    } else {
                        j++;
                    }
                }

                if (enemyKilled) {
                    enemies.erase(enemies.begin() + i);
                } else {
                    i++;
                }
            }

            if (hasFont) {
                int remainingEnemies = (enemiesToSpawnThisLevel - enemiesSpawnedSoFar) + enemies.size();
                uiText.setString("POZIOM: " + std::to_string(currentLevel) +
                                 "\nHP GRACZA: " + std::to_string(playerHp) +
                                 "\nPOZOSTALO WROGOW: " + std::to_string(remainingEnemies));
            }
        }


        window.clear(sf::Color(30, 30, 30));

        if(!isGameWon && !isGameOver){
            for (const auto& enemy : enemies) {
                window.draw(enemy.shape);
            }

            for (const auto& bullet : bullets) {
                window.draw(bullet.shape);
            }

            window.draw(player);
        }

        if (hasFont) {
            window.draw(uiText);
        }

        window.display();
    }

    return 0;
}

