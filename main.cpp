#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/box2d.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <format>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

 // The diameter of a football is roughly 22 cm
const float ballRadius = 0.11;

const int32 velocityIterations = 8;
const int32 positionIterations = 3;

b2Body* CreateBall(b2World& world, float radius)
{
    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(0, 4);
    b2Body* ball = world.CreateBody(&ballDef);
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

int main()
{
    // Create a window.
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "catcher");
    window.setVerticalSyncEnabled(true);

    // Gravity on Earth is 9.8067 m/s^2. Point it downwards (-Y).
    b2Vec2 gravity(0.0f, -9.8067);

    // Create the physics world.
    b2World world(gravity);

    // Create the ground.
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(0.0f, -0.5f);
    b2Body* groundBody = world.CreateBody(&groundDef);
    b2PolygonShape groundShape;
    groundShape.SetAsBox(5, 0.5);
    groundBody->CreateFixture(&groundShape, 0);

    // Left wall.
    b2BodyDef leftWallDef;
    leftWallDef.type = b2_staticBody;
    leftWallDef.position.Set(2.5f + 0.5f, 2.5f);
    b2Body* leftWallBody = world.CreateBody(&leftWallDef);
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(0.5, 2.5f);
    leftWallBody->CreateFixture(&leftWallShape, 0);

    // Right wall.
    b2BodyDef rightWallDef;
    rightWallDef.type = b2_staticBody;
    rightWallDef.position.Set(-2.5f - 0.5f, 2.5f);
    b2Body* rightWallBody = world.CreateBody(&rightWallDef);
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(0.5, 2.5f);
    rightWallBody->CreateFixture(&rightWallShape, 0);

    // Set up the camera.
    float width = 5;
    float height = 5;
    sf::View view(sf::FloatRect(-0.5 * width, 0, width, height));
    window.setView(view);

    std::vector<b2Body*> balls;

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle inputs
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // Create a ball
                    balls.push_back(CreateBall(world, ballRadius));
                }

                // Apply an impulse to the last created ball.
                if (!balls.empty())
                {
                    b2Body* ball = balls.back();
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

        float timeStep = clock.getElapsedTime().asSeconds();
        clock.restart();
        world.Step(timeStep, velocityIterations, positionIterations);

        // Render the scene.
        window.clear(sf::Color::Black);

        for (b2Body* ball : balls)
        {
            b2Vec2 ballPosition = ball->GetPosition();
            sf::CircleShape circleShape(ballRadius);
            circleShape.setOrigin(ballRadius, ballRadius);
            circleShape.setFillColor(sf::Color::Blue);
            circleShape.setPosition(ballPosition.x, height - ballPosition.y);

            window.draw(circleShape);
        }

        window.display();
    }
}