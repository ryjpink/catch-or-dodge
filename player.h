#pragma once // Ensure this file only gets included once

#include "entity.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <box2d/b2_world.h>

class Player : public Entity
{
  public:
    // Constructor
    Player(b2World &world); // (declaration)

    void SetMovingLeft(bool movingLeft);
    void SetMovingRight(bool movingRight);
    void Draw(sf::RenderWindow &window);
    void Update();

  private:
    float GetHorizontalVelocity();

    float _speed = 5; // m/s

    b2Body *_body;
    bool _movingLeft;
    bool _movingRight;
    sf::RectangleShape _rectangle;
};