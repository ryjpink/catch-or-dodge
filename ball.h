#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/b2_world.h>

class Ball
{
  public:
    Ball(b2World& world);

    void SetPosition(b2Vec2 position);
    void Draw(sf::RenderWindow& window);

  private:
    b2Body *_body;
    sf::CircleShape _shape;
};