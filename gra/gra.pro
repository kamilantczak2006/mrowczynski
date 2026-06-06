TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        GameObject.cpp \
        Student.cpp \
        main.cpp
        INCLUDEPATH +=C:\Users\Ja\Downloads\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\SFML-2.6.2\include
        LIBS+= -LC:\Users\Ja\Downloads\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\SFML-2.6.2\lib\
                -lsfml-graphics \
                -lsfml-window \
                -lsfml-system

HEADERS += \
    GameObject.h \
    Student.h
