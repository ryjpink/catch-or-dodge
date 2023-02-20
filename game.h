#pragma once

#include "scene.h"
#include "stage.h"

#include <SFML/Window/Event.hpp>
#include <memory>

class Game
{
  public:
    Game(Stage &stage) : _stage(stage)
    {
    }

    void SetScene(std::unique_ptr<Scene> nextScene)
    {
        _nextScene = std::move(nextScene);
    }

    Stage &GetStage()
    {
        return _stage;
    }

    void Update(float timeStep)
    {
        if (_nextScene)
        {
            _currentScene = std::move(_nextScene);
        }

        if (_currentScene)
        {
            _currentScene->Update(timeStep);
        }
    }
    void Draw(sf::RenderWindow &window)
    {
        if (_currentScene)
        {
            _currentScene->Draw(window);
        }
    }
    void OnKeyPressed(sf::Event::KeyEvent &event)
    {
        if (_currentScene)
        {
            _currentScene->OnKeyPressed(event);
        }
    }
    void OnKeyReleased(sf::Event::KeyEvent &event)
    {
        if (_currentScene)
        {
            _currentScene->OnKeyReleased(event);
        }
    }
    void OnTextEntered(sf::Event::TextEvent &event)
    {
        if (_currentScene)
        {
            _currentScene->OnTextEntered(event);
        }
    }

  private:
    Stage &_stage;
    std::unique_ptr<Scene> _currentScene;
    std::unique_ptr<Scene> _nextScene;
};