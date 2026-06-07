TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Bullet.cpp \
        Enemy.cpp \
        GameObject.cpp \
        Obstacle.cpp \
        Portal.cpp \
        ShooterEnemy.cpp \
        Student.cpp \
        main.cpp
        INCLUDEPATH +=C:\SFML-2.6.2\include
        LIBS+= -LC:\SFML-2.6.2\lib\
                -lsfml-graphics \
                -lsfml-window \
                -lsfml-system

HEADERS += \
    Bullet.h \
    Enemy.h \
    GameObject.h \
    Obstacle.h \
    Portal.h \
    ShooterEnemy.h \
    Student.h
