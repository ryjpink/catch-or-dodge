#include "player.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

Player::Player(b2World &world) // (definition)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(0, 0);
    _body = world.CreateBody(&bodyDef);

    float height = 1.8;            // m
    float width = 0.7;             // m
    float weight = 90;             // kg
    float volume = height * width; // m^2

    b2PolygonShape shape; // Shape for collision detection
    shape.SetAsBox(0.5 * width, 0.5 * height, b2Vec2(0, 0.5 * height), 0);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = weight / volume; // kg/m^2

    _body->CreateFixture(&fixtureDef);

    sf::Vector2f rectangleSize(width, height);
    sf::Color color(0xFF, 0xC0, 0xCB); // Pink
    _rectangle.setOrigin(sf::Vector2f(0.5 * width, 0));
    _rectangle.setSize(rectangleSize);
    _rectangle.setFillColor(color);
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
    if (_movingRight)
    {
        velocity += 1;
    }
    if (_movingLeft)
    {
        velocity -= 1;
    }
    return velocity;
}

void Player::Draw(sf::RenderWindow &window)
{
    b2Vec2 p = _body->GetPosition();
    _rectangle.setPosition(sf::Vector2f(p.x, p.y));
    window.draw(_rectangle);
}

void Player::Update()
{
    float vx = GetHorizontalVelocity();
    b2Vec2 v(vx, 0);
    _body->SetLinearVelocity(v);
}