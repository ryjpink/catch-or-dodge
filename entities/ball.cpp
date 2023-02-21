#include "entities/ball.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <numbers>
#include <random>
#include <span>

// The diameter of a football is roughly 22 cm
const float ballRadius = 0.5;

// Density of a football in kg / m^2:
//    Mass = 0.42 kg
//    Volume = pi * r^2 = pi * 0.11^2 = 0.0380132711 m^2
//  Density = Mass / Volume = 0.42 / 0.0380132711 = 11.0487729113
const float ballDensity = 11.0487729113;

std::mt19937 rng(std::random_device{}());

std::string goodTextures[] = {
    "assets/good_1.png", "assets/good_2.png", "assets/good_3.png", "assets/good_4.png",
    "assets/good_5.png", "assets/good_6.png", "assets/good_7.png", "assets/good_8.png",
    "assets/good_9.png", "assets/good_10.png", 
};
std::string badTextures[] = {
    "assets/bad_1.png", "assets/bad_2.png", "assets/bad_3.png", "assets/bad_4.png",
    "assets/bad_5.png",
};
std::string bossTextures[] = {
    "assets/boss_1.png", "assets/boss_2.png", "assets/boss_3.png",
};
std::string slowDownEffectTextures[] = {
    "assets/slow_down.png",
};
std::string speedUpEffectTextures[] = {
    "assets/speed_up.png",
};

std::span<std::string> GetTextureFilenames(BallType type)
{
    if (type == BallType::Good)
    {
        return goodTextures;
    }
    else if (type == BallType::Bad)
    {
        return badTextures;
    }
    else if (type == BallType::Boss)
    {
        return bossTextures;
    }
    else if (type == BallType::SlowDownEffect)
    {
        return slowDownEffectTextures;
    }
    else // (type == BallType::SpeedUpEffect)
    {
        return speedUpEffectTextures;
    }
}

std::string GetRandomTexture(BallType type)
{
    std::span<std::string> textureList = GetTextureFilenames(type);
    std::uniform_int_distribution<int> dist(0, textureList.size() - 1);
    int randomIndex = dist(rng);
    return textureList[randomIndex];
}

Ball::Ball(b2World &world, BallType type) : Entity(Type::Ball), _ballType(type)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData.pointer = (uintptr_t)(Entity *)this;
    _body = world.CreateBody(&bodyDef);

    std::string filename = GetRandomTexture(type);
    _texture.loadFromFile(filename);
    _sprite.setTexture(_texture);
    sf::Vector2u textureSize = _texture.getSize();
    _sprite.setTexture(_texture);
    _sprite.setOrigin(sf::Vector2f(0.5 * textureSize.x, 0.5 * textureSize.y));
    float aspectRatio = (float)textureSize.x / textureSize.y;
    float xScale = aspectRatio > 1 ? aspectRatio : 1;
    float yScale = aspectRatio > 1 ? 1 : 1 / aspectRatio;
    float xSize = xScale * ballRadius;
    float ySize = yScale * ballRadius;
    sf::Vector2f scale(xSize / textureSize.x, ySize / textureSize.y);
    _sprite.setScale(scale);

    b2PolygonShape shape;
    shape.SetAsBox(0.5 * xSize, 0.5 * ySize);

    b2FixtureDef ballFixture;
    ballFixture.shape = &shape;
    ballFixture.density = ballDensity;
    ballFixture.friction = 0.3;
    ballFixture.restitution = 0.8;
    _body->CreateFixture(&ballFixture);

    std::uniform_real_distribution<float> dist(-0.3, 0.3);
    float rotationsPerSecond = dist(rng);
    float radianPerSecond = 2 * std::numbers::pi * rotationsPerSecond;
    _body->SetAngularVelocity(radianPerSecond);
}

void Ball::SetPosition(b2Vec2 position)
{
    _body->SetTransform(position, 0);
}

void Ball::Draw(sf::RenderWindow &window)
{
    b2Vec2 p = _body->GetPosition();
    _sprite.setPosition(p.x, p.y);
    _sprite.setRotation(_body->GetAngle() * 180 / std::numbers::pi);
    window.draw(_sprite);
}

BallType Ball::GetBallType()
{
    return _ballType;
}