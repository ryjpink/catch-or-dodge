#pragma once // Ensure this file only gets included once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_world.h>

class Player
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

    b2Body *_body;
    bool _movingLeft;
    bool _movingRight;
    sf::RectangleShape _rectangle;
};