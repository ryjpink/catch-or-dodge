#include "player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/box2d.h>

#include <chrono>
#include <cstdio>
#include <thread>

using namespace std::chrono_literals;

float stageAspectRatio = 16 / 9.0;
float stageWidth = 20;                             // m
float stageHeight = stageWidth / stageAspectRatio; // m

// The diameter of a football is roughly 22 cm
const float ballRadius = 0.11;

// Options controlling the accuracy of the physics simulation
const int32 velocityIterations = 8;
const int32 positionIterations = 3;

b2Body *CreateBall(b2World &world, float radius)
{
    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(0, stageHeight);
    b2Body *ball = world.CreateBody(&ballDef);
    b2CircleShape ballShape;
    ballShape.m_radius = radius;
    b2FixtureDef ballFixture;
    ballFixture.shape = &ballShape;
    // Density of a football in kg / m^2:
    //    Mass = 0.42 kg
    //    Volume = pi * r^2 = pi * 0.11^2 = 0.0380132711 m^2
    //  Density = Mass / Volume = 0.42 / 0.0380132711 = 11.0487729113
    ballFixture.density = 11.0487729113;
    ballFixture.friction = 0.3;
    ballFixture.restitution = 0.8;
    ball->CreateFixture(&ballFixture);
    return ball;
}

void UpdateViewport(sf::RenderWindow &window)
{
    sf::Vector2u windowSize = window.getSize();

    float windowAspectRatio = windowSize.x / float(windowSize.y);
    float viewportWidth = stageWidth;
    float viewportHeight = stageHeight;
    if (windowAspectRatio > stageAspectRatio)
    {
        // Window wider than stage, letterbox left/right
        viewportWidth = stageHeight * windowAspectRatio;
    }
    else
    {
        // Stage wider than window, letterbox top/bottom
        viewportHeight = stageWidth / windowAspectRatio;
    }
    sf::View view;
    view.setSize(viewportWidth, -viewportHeight);
    view.setCenter(0, (stageHeight - viewportHeight) / 2 + viewportHeight / 2);
    window.setView(view);
}

int main()
{
    // Create a window.
    float initialWindowWidth = 1600;
    float initialWindowHeight = initialWindowWidth / stageAspectRatio;
    sf::RenderWindow window(sf::VideoMode(initialWindowWidth, initialWindowHeight), "catcher");
    window.setVerticalSyncEnabled(true);

    // Gravity on Earth is 9.8067 m/s^2. Point it downwards (-Y).
    b2Vec2 gravity(0.0f, -9.8067);

    // Create the physics world.
    b2World world(gravity);

    // Create the player.
    Player player(world);

    // Create the stage.
    sf::RectangleShape stage;
    stage.setOrigin(sf::Vector2f(0.5 * stageWidth, 0));
    stage.setSize(sf::Vector2f(stageWidth, stageHeight));
    stage.setFillColor(sf::Color::White);

    // Create the ground.
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(0.0f, -0.5f);
    b2Body *groundBody = world.CreateBody(&groundDef);
    b2PolygonShape groundShape;
    groundShape.SetAsBox(5, 0.5);
    groundBody->CreateFixture(&groundShape, 0);

    // Left wall.
    b2BodyDef leftWallDef;
    leftWallDef.type = b2_staticBody;
    leftWallDef.position.Set(2.5f + 0.5f, 2.5f);
    b2Body *leftWallBody = world.CreateBody(&leftWallDef);
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(0.5, 2.5f);
    leftWallBody->CreateFixture(&leftWallShape, 0);

    // Right wall.
    b2BodyDef rightWallDef;
    rightWallDef.type = b2_staticBody;
    rightWallDef.position.Set(-2.5f - 0.5f, 2.5f);
    b2Body *rightWallBody = world.CreateBody(&rightWallDef);
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(0.5, 2.5f);
    rightWallBody->CreateFixture(&rightWallShape, 0);

    // Set up the camera.
    UpdateViewport(window);

    std::vector<b2Body *> balls;

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle inputs
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                UpdateViewport(window);
            }
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    player.SetMovingLeft(true);
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    player.SetMovingRight(true);
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    player.SetMovingLeft(false);
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    player.SetMovingRight(false);
                }
                if (event.key.code == sf::Keyboard::Space)
                {
                    // Create a ball
                    balls.push_back(CreateBall(world, ballRadius));
                }

                // Apply an impulse to the last created ball.
                if (!balls.empty())
                {
                    b2Body *ball = balls.back();
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        b2Vec2 impulse(0, 1);
                        ball->ApplyLinearImpulse(impulse, ball->GetPosition(), true);
                    }
                    if (event.key.code == sf::Keyboard::Left)
                    {
                        b2Vec2 impulse(-1, 0);
                        ball->ApplyLinearImpulse(impulse, ball->GetPosition(), true);
                    }
                    if (event.key.code == sf::Keyboard::Right)
                    {
                        b2Vec2 impulse(1, 0);
                        ball->ApplyLinearImpulse(impulse, ball->GetPosition(), true);
                    }
                }
            }
        }

        player.Update();

        float timeStep = clock.getElapsedTime().asSeconds();
        clock.restart();
        world.Step(timeStep, velocityIterations, positionIterations);

        // Render the scene.
        window.clear(sf::Color::Black);
        window.draw(stage);

        for (b2Body *ball : balls)
        {
            b2Vec2 ballPosition = ball->GetPosition();
            sf::CircleShape circleShape(ballRadius);
            circleShape.setOrigin(ballRadius, ballRadius);
            circleShape.setFillColor(sf::Color::Blue);
            circleShape.setPosition(ballPosition.x, ballPosition.y);

            window.draw(circleShape);
        }
        player.Draw(window);
        window.display();
    }
}