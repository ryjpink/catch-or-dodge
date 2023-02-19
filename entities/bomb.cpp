#include "entities/bomb.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

// The diameter of a football is roughly 22 cm
const float bombRadius = 0.11;

// Density of a football in kg / m^2:
//    Mass = 0.42 kg
//    Volume = pi * r^2 = pi * 0.11^2 = 0.0380132711 m^2
//  Density = Mass / Volume = 0.42 / 0.0380132711 = 11.0487729113
const float bombDensity = 11.0487729113;

Bomb::Bomb(b2World &world) : Entity(Type::Bomb)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData.pointer = (uintptr_t)(Entity*)this;
    _body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = bombRadius;

    b2FixtureDef bombFixture;
    bombFixture.shape = &shape;
    bombFixture.density = bombDensity;
    bombFixture.friction = 0.3;
    bombFixture.restitution = 0.8;
    _body->CreateFixture(&bombFixture);

    _shape.setRadius(bombRadius);
    _shape.setOrigin(bombRadius, bombRadius);
    _shape.setFillColor(sf::Color::Red);
}

void Bomb::SetPosition(b2Vec2 position)
{
    _body->SetTransform(position, 0);
}

void Bomb::Draw(sf::RenderWindow &window)
{
    b2Vec2 p = _body->GetPosition();
    _shape.setPosition(p.x, p.y);
    window.draw(_shape);
}