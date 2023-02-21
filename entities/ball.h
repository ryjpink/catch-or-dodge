#pragma once

#include "entity.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <box2d/b2_world.h>

enum class BallType
{
    Good,
    Bad,
    Boss,
    SlowDownEffect,
    SpeedUpEffect,
};

class Ball : public Entity
{
  public:
    Ball(b2World& world, BallType type);

    void SetPosition(b2Vec2 position);
    void Draw(sf::RenderWindow& window);

    BallType GetBallType();

  private:
    BallType _ballType;
    b2Body *_body;
    sf::Texture _texture;
    sf::Sprite _sprite;
};