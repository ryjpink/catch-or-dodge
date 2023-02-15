#include "ball.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

// The diameter of a football is roughly 22 cm
const float ballRadius = 0.11;

// Density of a football in kg / m^2:
//    Mass = 0.42 kg
//    Volume = pi * r^2 = pi * 0.11^2 = 0.0380132711 m^2
//  Density = Mass / Volume = 0.42 / 0.0380132711 = 11.0487729113
const float ballDensity = 11.0487729113;

Ball::Ball(b2World &world) : Entity(Type::Ball)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData.pointer = (uintptr_t)(Entity*)this;
    _body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = ballRadius;

    b2FixtureDef ballFixture;
    ballFixture.shape = &shape;
    ballFixture.density = ballDensity;
    ballFixture.friction = 0.3;
    ballFixture.restitution = 0.8;
    _body->CreateFixture(&ballFixture);

    _shape.setRadius(ballRadius);
    _shape.setOrigin(ballRadius, ballRadius);
    _shape.setFillColor(sf::Color::Blue);
}

void Ball::SetPosition(b2Vec2 position)
{
    _body->SetTransform(position, 0);
}

void Ball::Draw(sf::RenderWindow &window)
{
    b2Vec2 p = _body->GetPosition();
    _shape.setPosition(p.x, p.y);
    window.draw(_shape);
}