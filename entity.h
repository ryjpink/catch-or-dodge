#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class Entity
{
  public:
    enum Type
    {
        Player,
        Ball,
        Bomb
    };

    Entity(Type type)
    {
        _type = type;
    }

    Type GetType()
    {
        return _type;
    }


    virtual void Draw(sf::RenderWindow &window) {}
    virtual void Update(float timeStep) {}
    void Destroy() { _isDestroyed = true; }
    bool IsDestroyed() { return _isDestroyed; }

  private:
    Type _type;
    bool _isDestroyed = false;
};