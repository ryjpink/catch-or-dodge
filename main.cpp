#include "game.h"
#include "stage.h"
#include "scenes/menu_scene.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

using namespace std::chrono_literals;

int main()
{
    auto stage = Stage::FromWidthAndAspectRatio(15, 16.0 / 9.0);

    auto game = Game(stage);
    game.SetScene(CreateMenuScene(game));

    // Create a window with the same aspect ratio as the stage.
    float initialWindowWidth = 1600;
    float initialWindowHeight = initialWindowWidth / stage.GetAspectRatio();
    sf::RenderWindow window(sf::VideoMode(initialWindowWidth, initialWindowHeight), "Catch or Dodge");
    window.setVerticalSyncEnabled(true);

    // Set up the camera.
    stage.FitToWindow(window);

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle inputs
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                stage.FitToWindow(window);
            }
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                game.OnKeyPressed(event.key);
            }
            if (event.type == sf::Event::KeyReleased)
            {
                game.OnKeyReleased(event.key);
            }
        }

        float timeStep = clock.getElapsedTime().asSeconds();
        clock.restart();

        game.Update(timeStep);

        // Render the scene.
        window.clear(sf::Color::Black);
        game.Draw(window);
        window.display();
    }
}