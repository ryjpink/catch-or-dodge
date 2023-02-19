#include "entities/player.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>

#include <cmath>
#include <numbers>

float height = 1.8;            // m
float width = 1.4425;          // m
float weight = 90;             // kg
float volume = height * width; // m^2

Player::Player(b2World &world) : Entity(Type::Player)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(0.5 * width, 0.5 * height);
    bodyDef.userData.pointer = (uintptr_t)(Entity *)this;
    _body = world.CreateBody(&bodyDef);

    b2PolygonShape shape; // Shape for collision detection
    shape.SetAsBox(0.5 * width, 0.5 * height, b2Vec2(0, 0), 0);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = weight / volume; // kg/m^2

    _body->CreateFixture(&fixtureDef);

    _texture.loadFromFile("assets/player.png");
    sf::Vector2u textureSize = _texture.getSize();
    _sprite.setTexture(_texture);
    _sprite.setOrigin(sf::Vector2f(0.5 * textureSize.x, 0.5 * textureSize.y));
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
}

void Player::Update(float timeStep)
{
    float vx = GetHorizontalVelocity();
    sf::Vector2u textureSize = _texture.getSize();
    if (vx > 0)
    {
        _sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    }
    else if (vx < 0)
    {
        _sprite.setTextureRect(sf::IntRect(textureSize.x, 0, -textureSize.x, textureSize.y));
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