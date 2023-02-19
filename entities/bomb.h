#pragma once

#include "entity.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/b2_world.h>

class Bomb : public Entity
{
  public:
    Bomb(b2World& world);

    void SetPosition(b2Vec2 position);
    void Draw(sf::RenderWindow& window);

  private:
    b2Body *_body;
    sf::CircleShape _shape;
};