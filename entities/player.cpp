#include "entities/player.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>

#include <cmath>
#include <numbers>

float height = 1.8;            // m
float width = 1.36736;         // m
float weight = 90;             // kg
float volume = height * width; // m^2

Player::Player(b2World &world) : Entity(Type::Player)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(0.5 * width, 0.5 * height);
    bodyDef.userData.pointer = (uintptr_t)(Entity *)this;
    _body = world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    fixtureDef.density = weight / volume; // kg/m^2
    _headShape.m_radius = 0.15 * width;
    _headShape.m_p = b2Vec2(0, -0.25 * height);
    fixtureDef.shape = &_headShape;
    _body->CreateFixture(&fixtureDef);
    _bodyShape.SetAsBox(0.15 * width, 0.4 * height, b2Vec2(0, 0.15 * height), 0);
    fixtureDef.shape = &_bodyShape;
    _body->CreateFixture(&fixtureDef);

    fixtureDef.isSensor = true;

    _rightFacingHandShape.SetAsBox(0.08 * width, 0.05 * height, b2Vec2(0.33 * width, -0.3 * height), 0);
    fixtureDef.shape = &_rightFacingHandShape;
    _rightFacingHandFixture = _body->CreateFixture(&fixtureDef);

    _leftFacingHandShape.SetAsBox(0.08 * width, 0.05 * height, b2Vec2(-0.33 * width, -0.3 * height), 0);
    fixtureDef.shape = &_leftFacingHandShape;
    _leftFacingHandFixture = _body->CreateFixture(&fixtureDef);

    _activeFixture = _rightFacingHandFixture;

    _texture.loadFromFile("assets/player.png");
    sf::Vector2u textureSize = _texture.getSize();
    _sprite.setTexture(_texture);
    _sprite.setOrigin(sf::Vector2f(0.4 * textureSize.x, 0.5 * textureSize.y));
    _baseScale = sf::Vector2f(width / textureSize.x, height / textureSize.y);
    _sprite.setScale(_baseScale);
}

void Player::SetPosition(b2Vec2 position)
{
    _body->SetTransform(position - b2Vec2(0, 0.5 * height), 0);
}

void Player::SetMovingLeft(bool movingLeft)
{
    _movingLeft = movingLeft;
}

void Player::SetMovingRight(bool movingRight)
{
    _movingRight = movingRight;
}

float Player::GetHorizontalVelocity()
{
    float velocity = 0;
    if (!IsDead())
    {
        if (_movingRight)
        {
            velocity += _speed;
        }
        if (_movingLeft)
        {
            velocity -= _speed;
        }
    }
    return velocity;
}

void Player::AddHealth(float amount)
{
    _lifeBar.Add(amount);
}

float Player::GetHealthRatio()
{
    return _lifeBar.GetRatio();
}

bool Player::IsDead()
{
    return _lifeBar.GetRatio() == 0;
}

void Player::Draw(sf::RenderWindow &window)
{
    b2Vec2 p = _body->GetPosition();
    _sprite.setPosition(p.x, p.y);
    _sprite.setRotation(_body->GetAngle() * 180 / std::numbers::pi);
    window.draw(_sprite);
    if (IsDead())
    {
        float shrinkScale = std::pow(0.35f, _timeSinceDeath);
        _sprite.setScale(_baseScale * shrinkScale);
    }

    if (_debugDrawColliders)
    {
        // Active hand sensor
        {
            sf::RectangleShape handSensor;
            handSensor.setFillColor(sf::Color(0xff00ff7f));
            b2AABB collisionAabb;
            _activeFixture->GetShape()->ComputeAABB(&collisionAabb, _body->GetTransform(), 0);
            handSensor.setPosition(collisionAabb.lowerBound.x, collisionAabb.lowerBound.y);
            b2Vec2 collisionAabbSize = 2 * collisionAabb.GetExtents();
            handSensor.setSize(sf::Vector2f(collisionAabbSize.x, collisionAabbSize.y));
            window.draw(handSensor);
        }

        // Head collider
        {
            sf::CircleShape head;
            head.setFillColor(sf::Color(0x7f7f7f7f));
            head.setRadius(_headShape.m_radius);
            head.setOrigin(_headShape.m_radius, _headShape.m_radius);
            b2Vec2 headPos = _body->GetWorldPoint(_headShape.m_p);
            head.setPosition(headPos.x, headPos.y);
            window.draw(head);
        }

        // Body collider
        {
            sf::RectangleShape body;
            body.setFillColor(sf::Color(0x7f7f7f7f));
            b2AABB collisionAabb;
            _bodyShape.ComputeAABB(&collisionAabb, _body->GetTransform(), 0);
            body.setPosition(collisionAabb.lowerBound.x, collisionAabb.lowerBound.y);
            b2Vec2 collisionAabbSize = 2 * collisionAabb.GetExtents();
            body.setSize(sf::Vector2f(collisionAabbSize.x, collisionAabbSize.y));
            window.draw(body);
        }
    }
}

void Player::Update(float timeStep)
{
    if (_timeUntilSpeedAlteringEffectFades > 0)
    {
        _timeUntilSpeedAlteringEffectFades -= timeStep;
    }
    else
    {
        _speed = _defaultSpeed;
    }
    float vx = GetHorizontalVelocity();
    sf::Vector2u textureSize = _texture.getSize();
    if (vx > 0)
    {
        _sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
        sf::Vector2u textureSize = _texture.getSize();
        _sprite.setOrigin(sf::Vector2f(0.4 * textureSize.x, 0.5 * textureSize.y));
        _activeFixture = _rightFacingHandFixture;
    }
    else if (vx < 0)
    {
        _sprite.setTextureRect(sf::IntRect(textureSize.x, 0, -textureSize.x, textureSize.y));
        sf::Vector2u textureSize = _texture.getSize();
        _sprite.setOrigin(sf::Vector2f(0.6 * textureSize.x, 0.5 * textureSize.y));
        _activeFixture = _leftFacingHandFixture;
    }
    b2Vec2 v(vx, 0);
    _body->SetLinearVelocity(v);
    if (IsDead())
    {
        float rotationsPerSecond = 1;
        float radianPerSecond = 2 * std::numbers::pi * rotationsPerSecond;
        _body->SetAngularVelocity(radianPerSecond);
        _timeSinceDeath += timeStep;
    }
}

b2Fixture *Player::GetActiveHandFixture()
{
    return _activeFixture;
}

void Player::ToggleDebugDrawColliders()
{
    _debugDrawColliders = !_debugDrawColliders;
}

void Player::ApplySlowDownEffect()
{
    _speed = _defaultSpeed * 0.5;
    _timeUntilSpeedAlteringEffectFades = 4;
}

void Player::ApplySpeedUpEffect()
{
    _speed = _defaultSpeed * 1.5;
    _timeUntilSpeedAlteringEffectFades = 8;
}