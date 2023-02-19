#include "stage.h"
#include <SFML/Graphics/Rect.hpp>

Stage Stage::FromWidthAndAspectRatio(float width, float aspectRatio)
{
    float height = width / aspectRatio;
    sf::Vector2f size(width, height);
    return Stage(size);
}

Stage::Stage(sf::Vector2f size)
{
    _bounds = sf::FloatRect(-0.5f * size, size);
    _aspectRatio = size.x / size.y;
}

sf::FloatRect Stage::GetBounds()
{
    return _bounds;
}

sf::Vector2f Stage::GetSize()
{
    return sf::Vector2f(_bounds.width, _bounds.height);
}

float Stage::GetAspectRatio()
{
    return _aspectRatio;
}

void Stage::FitToWindow(sf::RenderWindow &window)
{
    sf::Vector2u windowSize = window.getSize();

    float windowAspectRatio = windowSize.x / float(windowSize.y);
    float viewportWidth = _bounds.width;
    float viewportHeight = _bounds.height;
    if (windowAspectRatio > _aspectRatio)
    {
        // Window wider than stage, letterbox left/right
        viewportWidth = _bounds.height * windowAspectRatio;
    }
    else
    {
        // Stage wider than window, letterbox top/bottom
        viewportHeight = _bounds.width / windowAspectRatio;
    }
    sf::View view;
    view.setSize(viewportWidth, viewportHeight);
    view.setCenter(0, 0);
    window.setView(view);

    _windowToStage = sf::Transform::Identity;
    _windowToStage.translate(-0.5 * viewportWidth, -0.5 * viewportHeight);
    _windowToStage.scale(viewportWidth / windowSize.x, viewportHeight / windowSize.y);
    _stageToWindow = _windowToStage.getInverse();
}

sf::Vector2f Stage::WindowToStage(sf::Vector2f windowPoint)
{
    return _windowToStage.transformPoint(windowPoint);
}

sf::Vector2f Stage::StageToWindow(sf::Vector2f stagePoint)
{
    return _stageToWindow.transformPoint(stagePoint);
}
sf::FloatRect Stage::WindowToStage(sf::FloatRect windowRect)
{
    return _windowToStage.transformRect(windowRect);
}

sf::FloatRect Stage::StageToWindow(sf::FloatRect stageRect)
{
    return _stageToWindow.transformRect(stageRect);
}