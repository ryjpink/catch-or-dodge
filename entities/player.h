#pragma once // Ensure this file only gets included once

#include "entity.h"
#include "life_bar.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>

class Player : public Entity
{
  public:
    // Constructor
    Player(b2World &world); // (declaration)

    void SetPosition(b2Vec2 position);
    void SetMovingLeft(bool movingLeft);
    void SetMovingRight(bool movingRight);
    void ApplySlowDownEffect();
    void ApplySpeedUpEffect();
    void AddHealth(float amount);
    float GetHealthRatio();
    bool IsDead();
    void Draw(sf::RenderWindow &window);
    void Update(float timeStep);
    b2Fixture* GetActiveHandFixture();
    void ToggleDebugDrawColliders();

  private:
    float GetHorizontalVelocity();

    float _defaultSpeed = 5; // m/s
    float _speed = _defaultSpeed; // m/s
    float _timeUntilSpeedAlteringEffectFades = 0; // s

    b2Body *_body;
    bool _movingLeft = false;
    bool _movingRight = false;
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::Vector2f _baseScale;
    LifeBar _lifeBar;
    float _timeSinceDeath = 0;
    bool _debugDrawColliders = false;
    b2PolygonShape _leftFacingHandShape;
    b2PolygonShape _rightFacingHandShape;
    b2CircleShape _headShape;
    b2PolygonShape _bodyShape;
    b2Fixture *_leftFacingHandFixture;
    b2Fixture *_rightFacingHandFixture;
    b2Fixture *_activeFixture;
};