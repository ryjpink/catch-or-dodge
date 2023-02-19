#pragma once // Ensure this file only gets included once

#include "entity.h"
#include "life_bar.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_world.h>

class Player : public Entity
{
  public:
    // Constructor
    Player(b2World &world); // (declaration)

    void SetPosition(b2Vec2 position);
    void SetMovingLeft(bool movingLeft);
    void SetMovingRight(bool movingRight);
    void AddHealth(float amount);
    float GetHealthRatio();
    bool IsDead();
    void Draw(sf::RenderWindow &window);
    void Update(float timeStep);

  private:
    float GetHorizontalVelocity();

    float _speed = 5; // m/s

    b2Body *_body;
    bool _movingLeft = false;
    bool _movingRight = false;
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::Vector2f _baseScale;
    LifeBar _lifeBar;
    float _timeSinceDeath = 0;
};