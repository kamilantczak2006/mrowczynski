#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "ShooterEnemy.h"
#include "Portal.h"
#include "GameObject.h"
#include "Student.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "Boss.h"

using namespace std;

// System kolizji
void resolveSolidCollision(GameObject* entity, GameObject* obstacle) {
    sf::FloatRect eBounds = entity->getBounds();
    sf::FloatRect oBounds = obstacle->getBounds();
    sf::FloatRect overlap;

    if (eBounds.intersects(oBounds, overlap)) {
        if (overlap.width < overlap.height) {
            if (eBounds.left < oBounds.left) entity->forceMove(-overlap.width, 0);
            else entity->forceMove(overlap.width, 0);
        } else {
            if (eBounds.top < oBounds.top) entity->forceMove(0, -overlap.height);
            else entity->forceMove(0, overlap.height);
        }
    }
}

int main() {

    srand(time(NULL));

    // Tryb Fullscreen
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Lochy Uwalonych", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::Font font;
    bool hasFont = font.loadFromFile("arial.ttf");
    if (!hasFont) {
        cout << "UWAGA: Nie znaleziono pliku arial.ttf." << endl;
    }

    // GŁÓWNY KONTENER GRY
    vector<unique_ptr<GameObject>> allObjects;

    // Tworzenie gracza
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
    bool inMainMenu = true;
    bool isPaused = false;
    bool inUpgradeMenu = false;

    bool buffDoubleSpeed = false;
    bool buffDoubleDamage = false;
    bool unlockedNewStudent = false;
    bool playAsNewStudent = false;
    bool buffL4 = false;
    bool buffSciaga = false;
    bool buffDziekanka = false;

    // INTERFEJS
    sf::Text uiText;
    if (hasFont) {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        uiText.setPosition(20.0f, 20.0f);
    }

    // --- WŁASNY PRZYCISK WYJŚCIA "X" ---
    sf::Text exitButtonText;
    if (hasFont) {
        exitButtonText.setFont(font);
        exitButtonText.setCharacterSize(35);
        exitButtonText.setFillColor(sf::Color::Red);
        exitButtonText.setString("X");
        exitButtonText.setPosition(1230.0f, 10.0f); // Prawy górny róg
    }

    sf::Text titleText, controlsText, startButtonText, unlockText, newStudentBtnText;
    sf::RectangleShape startButton(sf::Vector2f(250.0f, 60.0f));
    sf::RectangleShape newStudentButton(sf::Vector2f(380.0f, 60.0f));

    if (hasFont) {
        titleText.setFont(font);
        titleText.setCharacterSize(60);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setString("LOCHY UWALONYCH");
        titleText.setPosition(1280.0f / 2.0f - 280.0f, 120.0f);

        controlsText.setFont(font);
        controlsText.setCharacterSize(26);
        controlsText.setFillColor(sf::Color::White);
        controlsText.setString("STEROWANIE:\nWSAD - Poruszanie sie\nSpacja - Dash\nStrzalki - Strzelanie\nESC - Pauza\n\nCel: Przetrwaj 4 pokoje i pokonaj Krola Warunkow!");
        controlsText.setPosition(1280.0f / 2.0f - 300.0f, 250.0f);

        startButton.setFillColor(sf::Color(0, 150, 0));
        startButtonText.setFont(font);
        startButtonText.setCharacterSize(30);
        startButtonText.setFillColor(sf::Color::White);
        startButtonText.setString("START");

        newStudentButton.setFillColor(sf::Color(150, 0, 150));
        newStudentBtnText.setFont(font);
        newStudentBtnText.setCharacterSize(20);
        newStudentBtnText.setFillColor(sf::Color::White);
        newStudentBtnText.setString("STUDENT DRUGIEGO STOPNIA\n(1 HP, Dmg x3)");

        unlockText.setFont(font);
        unlockText.setCharacterSize(28);
        unlockText.setFillColor(sf::Color::Cyan);
        unlockText.setString("Gra ukonczona - odblokowano nowego studenta!");
    }

    sf::RectangleShape returnButton(sf::Vector2f(300.0f, 60.0f));
    sf::Text returnBtnText;
    if (hasFont) {
        returnButton.setFillColor(sf::Color(100, 100, 100));
        returnButton.setPosition(1280.0f / 2.0f - 150.0f, 500.0f);
        returnBtnText.setFont(font);
        returnBtnText.setCharacterSize(24);
        returnBtnText.setFillColor(sf::Color::White);
        returnBtnText.setString("WROC DO MENU");
        returnBtnText.setPosition(1280.0f / 2.0f - 90.0f, 515.0f);
    }

    sf::RectangleShape pauseOverlay(sf::Vector2f(1280.0f, 720.0f));
    sf::Text pauseText;
    if (hasFont) {
        pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        pauseText.setFont(font);
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::Yellow);
        pauseText.setString("PAUZA\nWcisnij ESC, aby wrocic");
        pauseText.setPosition(1280.0f / 2.0f - 250.0f, 720.0f / 2.0f - 50.0f);
    }

    sf::Text upgTitle, upgText1, upgText2, upgText3;
    sf::RectangleShape upgBtn1(sf::Vector2f(280.0f, 180.0f));
    sf::RectangleShape upgBtn2(sf::Vector2f(280.0f, 180.0f));
    sf::RectangleShape upgBtn3(sf::Vector2f(280.0f, 180.0f));

    if (hasFont) {
        upgTitle.setFont(font);
        upgTitle.setCharacterSize(45);
        upgTitle.setFillColor(sf::Color::Yellow);
        upgTitle.setString("PORTAL ZAMKNIETY! WYBIERZ ULEPSZENIE:");
        upgTitle.setPosition(1280.0f / 2.0f - 450.0f, 150.0f);

        upgBtn1.setPosition(150.0f, 300.0f);
        upgBtn1.setFillColor(sf::Color(180, 30, 30));
        upgBtn2.setPosition(500.0f, 300.0f);
        upgBtn2.setFillColor(sf::Color(100, 50, 20));
        upgBtn3.setPosition(850.0f, 300.0f);
        upgBtn3.setFillColor(sf::Color(80, 20, 150));

        upgText1.setFont(font); upgText1.setCharacterSize(24); upgText1.setFillColor(sf::Color::White);
        upgText2.setFont(font); upgText2.setCharacterSize(24); upgText2.setFillColor(sf::Color::White);
        upgText3.setFont(font); upgText3.setCharacterSize(24); upgText3.setFillColor(sf::Color::White);
    }

    // Losowe generowanie początkowych przeszkód na planszy
    for (int i = 0; i < 5; i++) {
        float w = 50.0f + (rand() % 100); float h = 50.0f + (rand() % 100);
        float x = (rand() % 2 == 0) ? (100.0f + rand() % 400) : (800.0f + rand() % 400);
        float y = 100.0f + rand() % 500;
        allObjects.push_back(make_unique<Obstacle>(x, y, w, h));
    }

    // Wczytanie grafiki podłogi
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    if (bgTexture.loadFromFile("background.png")) {
        bgTexture.setSmooth(false);
        bgSprite.setTexture(bgTexture);

        // Skalowanie tła
        float scaleX = 1280.0f / bgTexture.getSize().x;
        float scaleY = 720.0f / bgTexture.getSize().y;
        bgSprite.setScale(scaleX, scaleY);
    }

    // ==========================================
    //            GŁÓWNA PĘTLA GRY
    // ==========================================
    while (window.isOpen()) {

        // dt - klatka
        float dt = clock.restart().asSeconds();
        bool playerEnteredPortal = false;

        // OBSŁUGA ZDARZEŃ
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float mx = static_cast<float>(mousePos.x);
            float my = static_cast<float>(mousePos.y);

            // Twarde wyjście z gry przyciskiem "X"
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (exitButtonText.getGlobalBounds().contains(mx, my)) {
                    window.close();
                }
            }

            // Obsługa kliknięć w Menu Głównym
            if (inMainMenu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (startButton.getGlobalBounds().contains(mx, my)) {
                    playAsNewStudent = false;
                    inMainMenu = false; clock.restart();
                } else if (unlockedNewStudent && newStudentButton.getGlobalBounds().contains(mx, my)) {
                    playAsNewStudent = true;
                    inMainMenu = false; clock.restart();

                    player->applySecondDegreeTexture();

                    // zmiana hp dla 2. postaci
                    int failsafe = 0;
                    while(player->getHp() > 1 && failsafe < 10) {
                        player->takeDamage(1);
                        player->update(2.0f);
                        failsafe++;
                    }
                }
            }

            // Ekran końcowy
            if ((isGameOver || isGameWon) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (returnButton.getGlobalBounds().contains(mx, my)) {
                    if (isGameWon) unlockedNewStudent = true;

                    // reset stanu gry
                    isGameOver = false;
                    isGameWon = false;
                    inMainMenu = true;
                    currentLevel = 1;
                    enemiesToSpawnThisLevel = 3;
                    enemiesSpawnedSoFar = 0;
                    transitionTimer = 0.0f;
                    portalSpawned = false;

                    buffDoubleSpeed = false;
                    buffDoubleDamage = false;
                    buffL4 = false;
                    buffSciaga = false;
                    buffDziekanka = false;

                    allObjects.clear();
                    auto newPlayerPtr = make_unique<Student>(1280.0f / 2.0f, 720.0f / 2.0f);
                    player = newPlayerPtr.get();

                    if (playAsNewStudent) player->applySecondDegreeTexture();

                    allObjects.push_back(move(newPlayerPtr));

                    // losowe przeszkody
                    for (int i = 0; i < 5; i++) {
                        float w = 50.0f + (rand() % 100); float h = 50.0f + (rand() % 100);
                        float x = (rand() % 2 == 0) ? (100.0f + rand() % 400) : (800.0f + rand() % 400);
                        float y = 100.0f + rand() % 500;
                        allObjects.push_back(make_unique<Obstacle>(x, y, w, h));
                    }
                    clock.restart();
                }
            }

            // Obsluga ulepszen
            if (inUpgradeMenu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (upgBtn1.getGlobalBounds().contains(mx, my)) {
                    if (playAsNewStudent) buffL4 = true; else player->heal(1);
                    inUpgradeMenu = false; clock.restart();
                } else if (upgBtn2.getGlobalBounds().contains(mx, my)) {
                    if (playAsNewStudent) buffSciaga = true; else buffDoubleSpeed = true;
                    inUpgradeMenu = false; clock.restart();
                } else if (upgBtn3.getGlobalBounds().contains(mx, my)) {
                    if (playAsNewStudent) buffDziekanka = true; else buffDoubleDamage = true;
                    inUpgradeMenu = false; clock.restart();
                }
            }

            // Pauzowanie gry
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (!inMainMenu && !inUpgradeMenu && !isGameOver && !isGameWon) {
                    isPaused = !isPaused;
                    if (!isPaused) clock.restart();
                }
            }
        }

        // --- RYSOWANIE EKRANOW STATYCZNYCH ---
        if (inMainMenu) {
            window.clear(sf::Color(20, 20, 20));
            if (hasFont) {
                window.draw(titleText); window.draw(controlsText);
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                float mx = static_cast<float>(mousePos.x);
                float my = static_cast<float>(mousePos.y);

                if (unlockedNewStudent) {
                    startButtonText.setString("ZWYKLY STUDENT");
                    startButtonText.setCharacterSize(22);
                    startButton.setPosition(1280.0f / 2.0f - 300.0f, 520.0f);
                    startButtonText.setPosition(1280.0f / 2.0f - 275.0f, 535.0f);

                    newStudentButton.setPosition(1280.0f / 2.0f + 50.0f, 520.0f);
                    newStudentBtnText.setPosition(1280.0f / 2.0f + 65.0f, 525.0f);

                    unlockText.setPosition(1280.0f / 2.0f - 270.0f, 460.0f);

                    newStudentButton.setFillColor(newStudentButton.getGlobalBounds().contains(mx, my) ? sf::Color(200, 0, 200) : sf::Color(150, 0, 150));
                    window.draw(unlockText);
                    window.draw(newStudentButton);
                    window.draw(newStudentBtnText);
                } else {
                    startButtonText.setString("START");
                    startButtonText.setCharacterSize(30);
                    startButton.setPosition(1280.0f / 2.0f - 125.0f, 520.0f);
                    startButtonText.setPosition(1280.0f / 2.0f - 45.0f, 530.0f);
                }

                startButton.setFillColor(startButton.getGlobalBounds().contains(mx, my) ? sf::Color(0, 200, 0) : sf::Color(0, 150, 0));
                window.draw(startButton); window.draw(startButtonText);

                // Przycisk X w menu
                window.draw(exitButtonText);
            }
            window.display();
            continue;
        }

        if (inUpgradeMenu) {
            window.clear(sf::Color(30, 20, 40));
            if (hasFont) {
                // ulepszenia dla 2. studenta
                if (playAsNewStudent) {
                    upgText1.setString("ZWOLNIENIE L4\n\nIgnoruje pierwszy\ncios na poziomie!");
                    upgText1.setPosition(165.0f, 320.0f);
                    upgText2.setString("SCIAGA\n\nPociski przenikaja\nprzez sciany!");
                    upgText2.setPosition(515.0f, 320.0f);
                    upgText3.setString("DZIEKANKA\n\nWrogowie i pociski\nzwalniaja o 50%!");
                    upgText3.setPosition(865.0f, 320.0f);
                } else {
                    // dla pierwszego
                    upgText1.setString("SERDUSZKO\n\nLeczy +1 HP\nOd razu!");
                    upgText1.setPosition(170.0f, 320.0f);
                    upgText2.setString("MOCNA KAWA\n\nPredkosc x1.5\n(Tylko ten poziom)");
                    upgText2.setPosition(520.0f, 320.0f);
                    upgText3.setString("FURIA\n\nObrazenia x2\n(Tylko ten poziom)");
                    upgText3.setPosition(870.0f, 320.0f);
                }

                window.draw(upgTitle);
                sf::Vector2i mPos = sf::Mouse::getPosition(window);
                upgBtn1.setOutlineThickness(upgBtn1.getGlobalBounds().contains(mPos.x, mPos.y) ? 5.0f : 0.0f);
                upgBtn2.setOutlineThickness(upgBtn2.getGlobalBounds().contains(mPos.x, mPos.y) ? 5.0f : 0.0f);
                upgBtn3.setOutlineThickness(upgBtn3.getGlobalBounds().contains(mPos.x, mPos.y) ? 5.0f : 0.0f);

                window.draw(upgBtn1); window.draw(upgText1);
                window.draw(upgBtn2); window.draw(upgText2);
                window.draw(upgBtn3); window.draw(upgText3);

                // Przycisk X w menu ulepszeń
                window.draw(exitButtonText);
            }
            window.display();
            continue;
        }

        // --- GLOWNA LOGIKA GRY ---
        if (!isGameWon && !isGameOver && !isPaused) {

            if (player->getHp() <= 0) {
                isGameOver = true;
                if (hasFont) {
                    uiText.setCharacterSize(40);
                    uiText.setPosition(1280.0f / 2.0f - 250.0f, 250.0f);
                    uiText.setString("GAME OVER!\nPrzegrales na poziomie: " + to_string(currentLevel));
                }
            }

            // Liczenie wrogów
            int activeEnemies = 0;
            for (auto& obj : allObjects) {
                if (dynamic_cast<Enemy*>(obj.get())) activeEnemies++;
            }

            // Pojawienie się portalu
            if (enemiesSpawnedSoFar >= enemiesToSpawnThisLevel && activeEnemies == 0) {
                if (!portalSpawned) {
                    allObjects.push_back(make_unique<Portal>(1280.0f / 2.0f, 720.0f / 2.0f));
                    portalSpawned = true;
                }
            }

            if (transitionTimer > 0.0f) transitionTimer -= dt;

            // Logika spawnowania nowych przeciwników
            if (enemiesSpawnedSoFar < enemiesToSpawnThisLevel && transitionTimer <= 0.0f) {
                if (enemySpawnClock.getElapsedTime().asSeconds() >= enemySpawnCooldown) {
                    float speed = 100.0f + (currentLevel * 20.0f);
                    int hp = 2;
                    int edge = rand() % 4;
                    float startX = 0, startY = 0;

                    if (edge == 0)      { startX = rand() % 1280; startY = -20; }
                    else if (edge == 1) { startX = rand() % 1280; startY = 740; }
                    else if (edge == 2) { startX = -20; startY = rand() % 720; }
                    else                { startX = 1300; startY = rand() % 720; }

                    // losowosc klas enemy
                    if (currentLevel >= 2 && rand() % 100 < 30) {
                        allObjects.push_back(make_unique<ShooterEnemy>(startX, startY, speed * 0.7f, hp));
                    } else {
                        allObjects.push_back(make_unique<Enemy>(startX, startY, speed, hp));
                    }
                    enemiesSpawnedSoFar++;
                    enemySpawnClock.restart();
                }
            }

            // Strzelanie gracz
            sf::Vector2f shootDir;
            if (player->tryShoot(shootDir)) {
                allObjects.push_back(make_unique<Bullet>(player->getPosition().x, player->getPosition().y, shootDir, 420.0f, sf::Color::Cyan, false));
            }

            // Strzelanie wrog
            vector<unique_ptr<GameObject>> enemyBullets;
            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    sf::Vector2f shootDir;
                    if (e->tryShoot(shootDir)) {
                        enemyBullets.push_back(make_unique<Bullet>(e->getPosition().x, e->getPosition().y, shootDir, 250.0f, sf::Color::Yellow, true));
                    }
                }
            }
            for (auto& b : enemyBullets) allObjects.push_back(move(b));

            // slugusy bossa
            vector<unique_ptr<GameObject>> summonedEnemies;
            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    if (e->trySummon()) {
                        float bx = e->getPosition().x; float by = e->getPosition().y;
                        summonedEnemies.push_back(make_unique<Enemy>(bx - 100.0f, by + 50.0f, 160.0f, 2));
                        summonedEnemies.push_back(make_unique<Enemy>(bx + 100.0f, by + 50.0f, 160.0f, 2));
                    }
                }
            }
            for (auto& newEnemy : summonedEnemies) allObjects.push_back(move(newEnemy));

            // aktualizacja celu dla wrogow
            for (auto& obj : allObjects) {
                if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
                    e->setTargetPosition(player->getPosition());
                }
            }

            // MODYFIKATORY CZASU
            for (auto& obj : allObjects) {
                float timeModifier = 1.0f;

                if (obj.get() == player && buffDoubleSpeed) {
                    timeModifier = 1.5f; // Kawa przyspiesza gracza
                }
                else if (dynamic_cast<Enemy*>(obj.get()) && buffDziekanka) {
                    timeModifier = 0.5f; // Dziekanka zwalnia wrogów
                }
                else if (Bullet* b = dynamic_cast<Bullet*>(obj.get())) {
                    if (b->isEnemyBullet && buffDziekanka) {
                        timeModifier = 0.5f; // Dziekanka zwalnia wrogie pociski
                    }
                }

                // wywolanie aktualizacji dla kazdego obiektu
                obj->update(dt * timeModifier);
            }

            // kolizja
            for (auto& obj : allObjects) {
                if (Obstacle* obs = dynamic_cast<Obstacle*>(obj.get())) {
                    resolveSolidCollision(player, obs);
                    for (auto& other : allObjects) {
                        if (Enemy* e = dynamic_cast<Enemy*>(other.get())) {
                            resolveSolidCollision(e, obs);
                        }
                    }
                }
            }

            // SYSTEM ZDERZEŃ I USUWANIA
            for (auto it = allObjects.begin(); it != allObjects.end(); ) {
                bool eraseObj = false;

                // zadawanie obrazen
                if (Enemy* e = dynamic_cast<Enemy*>(it->get())) {
                    if (e->getBounds().intersects(player->getBounds())) {
                        if (buffL4) { // Użycie tarczy z L4
                            buffL4 = false;
                            player->takeDamage(1); player->heal(1);
                        } else {
                            player->takeDamage(1);
                        }
                    }
                    if (e->getHp() <= 0) eraseObj = true;
                }
                // logika pociskow
                else if (Bullet* b = dynamic_cast<Bullet*>(it->get())) {
                    sf::Vector2f pos = b->getPosition();
                    bool hitObstacle = false;

                    for (auto& other : allObjects) {
                        if (Obstacle* obs = dynamic_cast<Obstacle*>(other.get())) {
                            if (b->getBounds().intersects(obs->getBounds())) {
                                if (buffSciaga && !b->isEnemyBullet) {
                                    break; // strzelanie przez sciany
                                }
                                hitObstacle = true; break;
                            }
                        }
                    }

                    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720 || hitObstacle) {
                        eraseObj = true;
                    } else {
                        if (b->isEnemyBullet) {
                            if (b->getBounds().intersects(player->getBounds())) {
                                if (buffL4) {
                                    buffL4 = false;
                                    player->takeDamage(1); player->heal(1);
                                } else {
                                    player->takeDamage(1);
                                }
                                eraseObj = true;
                            }
                        } else {
                            // sprawdzanie trafienia wroga
                            for (auto& other : allObjects) {
                                if (Enemy* e = dynamic_cast<Enemy*>(other.get())) {
                                    if (e->getHp() > 0 && e->getBounds().intersects(b->getBounds())) {
                                        int finalDamage = 1;
                                        if (buffDoubleDamage) finalDamage *= 2;
                                        if (playAsNewStudent) finalDamage *= 3;

                                        e->takeDamage(finalDamage);
                                        eraseObj = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                // Portal
                else if (Portal* p = dynamic_cast<Portal*>(it->get())) {
                    if (p->getBounds().intersects(player->getBounds())) {
                        playerEnteredPortal = true;
                        eraseObj = true;
                    }
                }

                if (eraseObj) it = allObjects.erase(it);
                else ++it;
            }

            // HUD
            if (hasFont && !isGameOver && !isGameWon) {
                uiText.setCharacterSize(24);
                uiText.setPosition(20.0f, 20.0f);
                int remainingEnemies = (enemiesToSpawnThisLevel - enemiesSpawnedSoFar) + activeEnemies;
                string activeBuffs = "";

                if (buffDoubleSpeed) activeBuffs += "\nAKTYWNY BOOST: Kawa (x1.5 Ruch)";
                if (buffDoubleDamage) activeBuffs += "\nAKTYWNY BOOST: Furia (x2 Obrazenia)";
                if (buffL4) activeBuffs += "\nAKTYWNY BOOST: Zwolnienie L4 (Tarcza aktywna)";
                if (buffSciaga) activeBuffs += "\nAKTYWNY BOOST: Sciaga (Pociski przenikaja sciany)";
                if (buffDziekanka) activeBuffs += "\nAKTYWNY BOOST: Dziekanka (Wrogowie zwolnieni)";

                uiText.setString("POZIOM: " + to_string(currentLevel) +
                                 "\nHP GRACZA: " + to_string(player->getHp()) +
                                 "\nPOZOSTALO WROGOW: " + to_string(remainingEnemies) + activeBuffs);
            }
        }

        // ZMIANY POZIOMU I WYGRANA
        if (playerEnteredPortal) {
            if (currentLevel == 5) {
                isGameWon = true;
                if (hasFont) {
                    uiText.setCharacterSize(40);
                    uiText.setPosition(1280.0f / 2.0f - 350.0f, 250.0f);
                    uiText.setString("GRATULACJE! UKONCZYLES CALA GRE!\nPokonales Krola Warunkow!");
                }
            } else {
                currentLevel++;
                enemiesSpawnedSoFar = 0;
                portalSpawned = false;
                transitionTimer = 2.0f;

                // reset buff
                buffDoubleSpeed = false;
                buffDoubleDamage = false;
                buffL4 = false;
                buffSciaga = false;
                buffDziekanka = false;

                if (currentLevel <= 5) inUpgradeMenu = true;

                if (currentLevel == 2) enemiesToSpawnThisLevel = 5;
                else if (currentLevel == 3) enemiesToSpawnThisLevel = 8;
                else if (currentLevel == 4) enemiesToSpawnThisLevel = 12;
                else if (currentLevel == 5) {
                    enemiesToSpawnThisLevel = 1;
                    int bossHp = playAsNewStudent ? 100 : 50;
                    allObjects.push_back(make_unique<Boss>(1280.0f / 2.0f, 150.0f, bossHp));
                    enemiesSpawnedSoFar = 1;
                }

                // Usuwanie smieci
                for (auto it = allObjects.begin(); it != allObjects.end(); ) {
                    if (dynamic_cast<Obstacle*>(it->get()) || dynamic_cast<Bullet*>(it->get())) {
                        it = allObjects.erase(it);
                    } else ++it;
                }

                // nowe przeszkody
                if(currentLevel != 5){
                    for (int i = 0; i < 5; i++) {
                        float w = 50.0f + (rand() % 100); float h = 50.0f + (rand() % 100);
                        float x = (rand() % 2 == 0) ? (100.0f + rand() % 400) : (800.0f + rand() % 400);
                        float y = 100.0f + rand() % 500;
                        allObjects.push_back(make_unique<Obstacle>(x, y, w, h));
                    }
                }

                if (inUpgradeMenu) continue;
            }
        }

        // --- RENDEROWANIE EKRANU GRY ---
        window.clear(sf::Color(30, 30, 30));

        if (!isGameWon && !isGameOver) {
            window.draw(bgSprite);

            for (auto& obj : allObjects) obj->draw(window);
        }

        if (hasFont) {
            window.draw(uiText);

            if (transitionTimer > 0.0f && !isGameWon && !isGameOver) {
                sf::Text transitionText;
                transitionText.setFont(font);
                transitionText.setCharacterSize(50);
                transitionText.setFillColor(sf::Color::Yellow);
                transitionText.setString("PRZECHODZISZ NA POZIOM " + to_string(currentLevel) + "!");
                transitionText.setPosition(1280.0f / 2.0f - 350.0f, 720.0f / 2.0f - 50.0f);
                window.draw(transitionText);
            }

            // pasek zycia bossa
            if (currentLevel == 5 && !isGameWon && !isGameOver) {
                Boss* bossPtr = nullptr;
                for (auto& obj : allObjects) {
                    if (Boss* b = dynamic_cast<Boss*>(obj.get())) { bossPtr = b; break; }
                }

                if (bossPtr && bossPtr->getHp() > 0) {
                    sf::RectangleShape bg(sf::Vector2f(800.0f, 30.0f));
                    bg.setPosition(1280.0f / 2.0f - 400.0f, 20.0f);
                    bg.setFillColor(sf::Color(80, 0, 0));
                    bg.setOutlineColor(sf::Color::White); bg.setOutlineThickness(3.0f);

                    float hpPercent = (float)bossPtr->getHp() / bossPtr->maxHp;
                    if (hpPercent < 0) hpPercent = 0.0f;
                    sf::RectangleShape fg(sf::Vector2f(800.0f * hpPercent, 30.0f));
                    fg.setPosition(1280.0f / 2.0f - 400.0f, 20.0f);
                    fg.setFillColor(sf::Color::Red);

                    window.draw(bg); window.draw(fg);

                    sf::Text bossText;
                    bossText.setFont(font); bossText.setCharacterSize(22); bossText.setFillColor(sf::Color::White);
                    bossText.setString("KROL WARUNKOW - HP: " + to_string(bossPtr->getHp()));
                    bossText.setPosition(1280.0f / 2.0f - 140.0f, 22.0f);
                    window.draw(bossText);
                }
            }

            if (isGameOver || isGameWon) {
                sf::Vector2i mPos = sf::Mouse::getPosition(window);
                returnButton.setFillColor(returnButton.getGlobalBounds().contains(mPos.x, mPos.y) ? sf::Color(130, 130, 130) : sf::Color(100, 100, 100));
                window.draw(returnButton);
                window.draw(returnBtnText);
            }

            if (isPaused) {
                window.draw(pauseOverlay);
                window.draw(pauseText);
            }

            // Przycisk X podczas gry i na ekranach końcowych/pauzy
            window.draw(exitButtonText);
        }

        window.display();
    }

    return 0;
}
