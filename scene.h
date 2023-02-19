#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Scene
{
  public:
    virtual void Update(float timeStep)
    {
    }
    virtual void Draw(sf::RenderWindow &window)
    {
    }
    virtual void OnKeyPressed(sf::Event::KeyEvent &event)
    {
    }
    virtual void OnKeyReleased(sf::Event::KeyEvent &event)
    {
    }
};