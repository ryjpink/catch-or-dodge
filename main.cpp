#include "player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <box2d/box2d.h>

// Defines the size of the game stage (playable area).
float stageAspectRatio = 16 / 9.0;
float stageWidth = 20;                             // m
float stageHeight = stageWidth / stageAspectRatio; // m

// Options controlling the accuracy of the physics simulation
const int32 velocityIterations = 8;
const int32 positionIterations = 3;

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

    // Set up the camera.
    UpdateViewport(window);

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
            }
        }

        float timeStep = clock.getElapsedTime().asSeconds();
        clock.restart();

        // Update the game.
        player.Update();
        world.Step(timeStep, velocityIterations, positionIterations);

        // Render the scene.
        window.clear(sf::Color::Black);
        window.draw(stage);
        player.Draw(window);
        window.display();
    }
}