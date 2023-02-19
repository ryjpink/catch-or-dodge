#pragma once


#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Stage
{
  public:
    Stage(sf::Vector2f size);

    sf::FloatRect GetBounds();
    sf::Vector2f GetSize();
    float GetAspectRatio();

    // Tries to center the stage within the window,
    // using as much space as possible while preserving
    // the stage's aspect ratio.
    void FitToWindow(sf::RenderWindow &window);

    sf::Vector2f WindowToStage(sf::Vector2f windowPoint);
    sf::Vector2f StageToWindow(sf::Vector2f stagePoint);
    sf::FloatRect WindowToStage(sf::FloatRect windowRect);
    sf::FloatRect StageToWindow(sf::FloatRect stageRect);

    // Creates a stage with a specific width and aspect ratio.
    static Stage FromWidthAndAspectRatio(float width, float aspectRatio);

  private:
    sf::FloatRect _bounds;
    float _aspectRatio;
    sf::Transform _windowToStage; // defines where the stage is drawn in the window
    sf::Transform _stageToWindow;
};