#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#include "ShooterEnemy.h"
#include "Portal.h"
#include "GameObject.h"
#include "Student.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Obstacle.h"

// Funkcja wypychająca obiekty z przeszkód
void resolveSolidCollision(GameObject* entity, GameObject* obstacle) {
    sf::FloatRect eBounds = entity->getBounds();
    sf::FloatRect oBounds = obstacle->getBounds();
    sf::FloatRect overlap;


    if (eBounds.intersects(oBounds, overlap)) {
        // Wypychamy obiekt po krótszej osi przecięcia
        if (overlap.width < overlap.height) {
            if (eBounds.left < oBounds.left) entity->forceMove(-overlap.width, 0);
            else entity->forceMove(overlap.width, 0);
        } else {
            if (eBounds.top < oBounds.top) entity->forceMove(0, -overlap.height);
            else entity->forceMove(0, overlap.height);
        }
    }
}

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
    bool portalSpawned = false;
    float transitionTimer = 0.0f;



    // Generowanie przeszkód dla poziomu 1
    for (int i = 0; i < 5; i++) {
        float w = 50.0f + (rand() % 100);
        float h = 50.0f + (rand() % 100);
        // Unikamy środka mapy (gdzie pojawia się gracz)
        float x = (rand() % 2 == 0) ? (100.0f + rand() % 400) : (800.0f + rand() % 400);
        float y = 100.0f + rand() % 500;
        allObjects.push_back(make_unique<Obstacle>(x, y, w, h));
    }

    // Główna pętla
    while (window.isOpen()) {


        float dt = clock.restart().asSeconds();
        bool playerEnteredPortal = false;

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

            // Jeśli pokonano wszystkich wrogów na obecnym poziomie:
            if (enemiesSpawnedSoFar >= enemiesToSpawnThisLevel && activeEnemies == 0) {
                if (currentLevel < 3) {
                    // Spawnuje portal tylko raz
                    if (!portalSpawned) {
                        allObjects.push_back(make_unique<Portal>(1280.0f / 2.0f, 720.0f / 2.0f));
                        portalSpawned = true;
                    }
                } else {
                    // Koniec gry na najwyższym poziomie
                    isGameWon = true;
                    if (hasFont) {
                        uiText.setCharacterSize(36);
                        uiText.setPosition(1280.0f / 4.0f, 720.0f / 3.0f);
                        uiText.setString("GRATULACJE! UKONCZYLES CALA GRE!\nKONCOWY POZIOM: " + to_string(currentLevel) + "\nZamknij okno, aby wyjsc.");
                    } else cout << "WYGRANA!" << endl;
                }
            }

            // Odejmujemy czas od licznika napisu
            if (transitionTimer > 0.0f) {
                transitionTimer -= dt;
            }

            // 3. Spawnowanie Przeciwników (dodany warunek transitionTimer <= 0.0f)
            if (enemiesSpawnedSoFar < enemiesToSpawnThisLevel && transitionTimer <= 0.0f) {
                if (enemySpawnClock.getElapsedTime().asSeconds() >= enemySpawnCooldown) {
                    // ... (tutaj reszta Twojego kodu losująca pozycję i tworząca wroga) ...
                    float speed = 100.0f + (currentLevel * 20.0f);
                    int hp = 2;


                    int edge = rand() % 4;
                    float startX = 0, startY = 0;
                    if (edge == 0)      { startX = rand() % 1280; startY = -20; }
                    else if (edge == 1) { startX = rand() % 1280; startY = 740; }
                    else if (edge == 2) { startX = -20; startY = rand() % 720; }
                    else                { startX = 1300; startY = rand() % 720; }


                    if (currentLevel >= 2 && rand() % 100 < 30) {

                        allObjects.push_back(make_unique<ShooterEnemy>(startX, startY, speed * 0.7f, hp));
                    } else {

                        allObjects.push_back(make_unique<Enemy>(startX, startY, speed, hp));
                    }

                    allObjects.push_back(make_unique<Enemy>(startX, startY, speed, hp));
                    enemiesSpawnedSoFar++;
                    enemySpawnClock.restart();
                }
            }

            // Strzelanie
            sf::Vector2f shootDir;
            if (player->tryShoot(shootDir)) {
                allObjects.push_back(make_unique<Bullet>(player->getPosition().x, player->getPosition().y, shootDir, 420.0f, sf::Color::Cyan, false));
            }

            //Strzelanie Przeciwników
            vector<unique_ptr<GameObject>> enemyBullets;
            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    sf::Vector2f shootDir;
                    if (e->tryShoot(shootDir)) {
                        enemyBullets.push_back(make_unique<Bullet>(e->getPosition().x, e->getPosition().y, shootDir, 250.0f, sf::Color::Yellow, true));
                    }
                }
            }

            // Dodanie zebranych pocisków na mapę
            for (auto& b : enemyBullets) {
                allObjects.push_back(move(b));
            }
            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    e->setTargetPosition(player->getPosition());
                }
            }


            for (auto& obj : allObjects) {
                obj->update(dt);
            }


            //FIZYKA BARYKAD





            for (auto& obj : allObjects) {
                if (Obstacle* obs = dynamic_cast<Obstacle*>(obj.get())) {
                    // Sprawdzamy zderzenie gracza z tą przeszkodą
                    resolveSolidCollision(player, obs);

                    // Sprawdzamy zderzenie każdego wroga z tą przeszkodą
                    for (auto& other : allObjects) {
                        if (Enemy* e = dynamic_cast<Enemy*>(other.get())) {
                            resolveSolidCollision(e, obs);
                        }
                    }
                }
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
                    bool hitObstacle = false;

                    // Najpierw sprawdzamy, czy pocisk uderzył w ścianę
                    for (auto& other : allObjects) {
                        if (Obstacle* obs = dynamic_cast<Obstacle*>(other.get())) {
                            if (b->getBounds().intersects(obs->getBounds())) {
                                hitObstacle = true;
                                break;
                            }
                        }
                    }

                    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720 || hitObstacle) {
                        eraseObj = true; // Usuwamy pocisk, bo wybuchł na przeszkodzie lub wyleciał z mapy
                    } else {

                        if (b->isEnemyBullet) {
                            // POCISK WROGA - rani tylko gracza
                            if (b->getBounds().intersects(player->getBounds())) {
                                player->takeDamage(1);
                                eraseObj = true;
                            }
                        } else {
                            // POCISK GRACZA - rani wrogów
                            for (auto& other : allObjects) {
                                if (Enemy* e = dynamic_cast<Enemy*>(other.get())) {
                                    if (e->getHp() > 0 && e->getBounds().intersects(b->getBounds())) {
                                        e->takeDamage(1);
                                        eraseObj = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }



                else if (Portal* p = dynamic_cast<Portal*>(it->get())) {
                    if (p->getBounds().intersects(player->getBounds())) {
                        playerEnteredPortal = true;
                        eraseObj = true;
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



        if (playerEnteredPortal) {
            currentLevel++;
            enemiesSpawnedSoFar = 0;
            portalSpawned = false;
            transitionTimer = 2.0f;

            if (currentLevel == 2) enemiesToSpawnThisLevel = 5;
            else if (currentLevel == 3) enemiesToSpawnThisLevel = 8;

            // USUWANIE starych przeszkód i pocisków
            for (auto it = allObjects.begin(); it != allObjects.end(); ) {
                if (dynamic_cast<Obstacle*>(it->get()) || dynamic_cast<Bullet*>(it->get())) {
                    it = allObjects.erase(it);
                } else {
                    ++it;
                }
            }

            // GENEROWANIE przeszkód
            for (int i = 0; i < 4 + currentLevel; i++) {
                float w = 50.0f + (rand() % 100);
                float h = 50.0f + (rand() % 100);
                float x = (rand() % 2 == 0) ? (100.0f + rand() % 400) : (800.0f + rand() % 400);
                float y = 100.0f + rand() % 500;
                allObjects.push_back(make_unique<Obstacle>(x, y, w, h));
            }
        }


        // --- RYSOWANIE ---
        window.clear(sf::Color(30, 30, 30));

        if (!isGameWon && !isGameOver) {
            for (auto& obj : allObjects) {
                obj->draw(window);
            }
        }

        if (hasFont) {
            window.draw(uiText); // Rysuje zwykłe statystyki w lewym górnym rogu

            // Rysuje wielki napis na środku, jeśli licznik działa
            if (transitionTimer > 0.0f) {
                sf::Text transitionText;
                transitionText.setFont(font);
                transitionText.setCharacterSize(50);
                transitionText.setFillColor(sf::Color::Yellow);
                transitionText.setString("PRZECHODZISZ NA POZIOM " + to_string(currentLevel) + "!");

                // Wyśrodkowanie tekstu na oko
                transitionText.setPosition(1280.0f / 2.0f - 350.0f, 720.0f / 2.0f - 50.0f);

                window.draw(transitionText);
            }
        }

        window.display();
    }

    return 0;
}
