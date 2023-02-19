#include "text.h"

sf::FloatRect MeasureText(Stage &stage, sf::Text &text)
{
    sf::FloatRect textBoundsInWindow = text.getLocalBounds();
    sf::FloatRect textBoundsInStage = stage.WindowToStage(textBoundsInWindow);
    return textBoundsInStage;
}

void DrawText(sf::RenderWindow &window, Stage &stage, sf::Text &text, sf::FloatRect bounds, TextAlignment alignment)
{
    sf::FloatRect textBoundsInStage = MeasureText(stage, text);
    sf::Vector2f textPositionInStage;
    textPositionInStage.y = bounds.top;
    textPositionInStage.x = bounds.left;
    if (alignment == TextAlignment::Right)
    {
        textPositionInStage.x = bounds.left + bounds.width - textBoundsInStage.width;
    }
    else if (alignment == TextAlignment::Center)
    {
        textPositionInStage.x = bounds.left + 0.5f * (bounds.width - textBoundsInStage.width);
    }
    sf::Vector2f textPositionInWindow = stage.StageToWindow(textPositionInStage);
    text.setPosition(textPositionInWindow);

    sf::Vector2u windowSize = window.getSize();
    sf::View windowView(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    sf::View previousView = window.getView();
    window.setView(windowView);
    window.draw(text);
    window.setView(previousView);
}