#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <SFML/Graphics.hpp>

// Nasze klasy obiektowe
#include "GameObject.h"
#include "Student.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalonych");
    window.setFramerateLimit(60);


    sf::Font font;
    bool hasFont = font.loadFromFile("arial.ttf");
    if (!hasFont) {
        cout << "UWAGA: Nie znaleziono pliku arial.ttf. UI bedzie wyswietlane w konsoli!" << endl;
    }

    sf::Text uiText;
    if (hasFont) {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        uiText.setPosition(20.0f, 20.0f);
    }

    // --- GŁÓWNY KONTENER GRY ---
    vector<unique_ptr<GameObject>> allObjects;


    auto playerPtr = make_unique<Student>(1280.0f / 2.0f, 720.0f / 2.0f);
    Student* player = playerPtr.get();
    allObjects.push_back(move(playerPtr));


    sf::Clock clock;
    sf::Clock enemySpawnClock;
    float enemySpawnCooldown = 1.5f;

    int currentLevel = 1;
    int enemiesToSpawnThisLevel = 3;
    int enemiesSpawnedSoFar = 0;

    bool isGameWon = false;
    bool isGameOver = false;

    // Główna pętla
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (!isGameWon && !isGameOver) {

            // Sprawdzenie stanu gracza
            if (player->getHp() <= 0) {
                isGameOver = true;
                if (hasFont) {
                    uiText.setCharacterSize(36);
                    uiText.setPosition(1280.0f / 4.0f, 720.0f / 3.0f);
                    uiText.setString("GAME OVER!\nPrzegrales na poziomie: " + to_string(currentLevel) + "\nZamknij okno, aby wyjsc.");
                } else cout << "GAME OVER!" << endl;
            }

            // Logika Fal
            int activeEnemies = 0;
            for (auto& obj : allObjects) {
                if (dynamic_cast<Enemy*>(obj.get())) activeEnemies++;
            }

            if (enemiesSpawnedSoFar >= enemiesToSpawnThisLevel && activeEnemies == 0) {
                currentLevel++;
                enemiesSpawnedSoFar = 0;

                if (currentLevel == 2) enemiesToSpawnThisLevel = 5;
                else if (currentLevel == 3) enemiesToSpawnThisLevel = 8;
                else {
                    isGameWon = true;
                    if (hasFont) {
                        uiText.setCharacterSize(36);
                        uiText.setPosition(1280.0f / 4.0f, 720.0f / 3.0f);
                        uiText.setString("GRATULACJE! UKONCZYLES CALA GRE!\nKONCOWY POZIOM: " + to_string(currentLevel - 1) + "\nZamknij okno, aby wyjsc.");
                    } else cout << "WYGRANA!" << endl;
                }
            }

            // Spawnowanie Przeciwników
            if (enemiesSpawnedSoFar < enemiesToSpawnThisLevel) {
                if (enemySpawnClock.getElapsedTime().asSeconds() >= enemySpawnCooldown) {
                    float speed = 100.0f + (currentLevel * 20.0f);
                    int hp = 2;


                    int edge = rand() % 4;
                    float startX = 0, startY = 0;
                    if (edge == 0)      { startX = rand() % 1280; startY = -20; }
                    else if (edge == 1) { startX = rand() % 1280; startY = 740; }
                    else if (edge == 2) { startX = -20; startY = rand() % 720; }
                    else                { startX = 1300; startY = rand() % 720; }

                    allObjects.push_back(make_unique<Enemy>(startX, startY, speed, hp));
                    enemiesSpawnedSoFar++;
                    enemySpawnClock.restart();
                }
            }

            // Strzelanie
            sf::Vector2f shootDir;
            if (player->tryShoot(shootDir)) {
                allObjects.push_back(make_unique<Bullet>(player->getPosition().x, player->getPosition().y, shootDir, 420.0f));
            }


            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    e->setTargetPosition(player->getPosition());
                }
            }


            for (auto& obj : allObjects) {
                obj->update(dt);
            }


            for (auto it = allObjects.begin(); it != allObjects.end(); ) {
                bool eraseObj = false;


                if (Enemy* e = dynamic_cast<Enemy*>(it->get())) {
                    if (e->getBounds().intersects(player->getBounds())) {
                        player->takeDamage(1); // Student rani się o wroga
                    }
                    if (e->getHp() <= 0) eraseObj = true; // Wróg zginął
                }

                else if (Bullet* b = dynamic_cast<Bullet*>(it->get())) {
                    sf::Vector2f pos = b->getPosition();
                    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) {
                        eraseObj = true; // Usuwamy pocisk, który wyleciał za mapę
                    } else {
                        // kolizja pocisku ze wszystkimi innymi obiektami
                        for (auto& other : allObjects) {
                            if (Enemy* e = dynamic_cast<Enemy*>(other.get())) {
                                if (e->getHp() > 0 && e->getBounds().intersects(b->getBounds())) {
                                    e->takeDamage(1);
                                    eraseObj = true; // Usuwamy pocisk po trafieniu
                                    break;
                                }
                            }
                        }
                    }
                }


                if (eraseObj) {
                    it = allObjects.erase(it);
                } else {
                    ++it;
                }
            }


            if (hasFont && !isGameOver && !isGameWon) {
                int remainingEnemies = (enemiesToSpawnThisLevel - enemiesSpawnedSoFar) + activeEnemies;
                uiText.setString("POZIOM: " + to_string(currentLevel) +
                                 "\nHP GRACZA: " + to_string(player->getHp()) +
                                 "\nPOZOSTALO WROGOW: " + to_string(remainingEnemies));
            }
        }

        // --- RYSOWANIE ---
        window.clear(sf::Color(30, 30, 30));


        if (!isGameWon && !isGameOver) {
            for (auto& obj : allObjects) {
                obj->draw(window);
            }
        }

        if (hasFont) window.draw(uiText);

        window.display();
    }

    return 0;
}
