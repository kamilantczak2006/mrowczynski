#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
  
protected:
    sf::RectangleShape shape; 
    sf::Vector2f position;    
    float speed;              
    float rotationSpeed;      

    
public:
   
    GameObject(float startX, float startY, float startSpeed);

  
    virtual ~GameObject() = default;


    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    void forceMove(float dx, float dy);
    
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};
