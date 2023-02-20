#include "ui/text.h"

#include <SFML/System/Vector2.hpp>

sf::Vector2f MeasureText(Stage &stage, sf::Text &text)
{
    sf::Vector2f globalEndPos = text.findCharacterPos(text.getString().getSize());
    sf::Vector2f localEndPos = text.getInverseTransform().transformPoint(globalEndPos);
    float lineSpacing = text.getFont()->getLineSpacing(text.getCharacterSize()) * text.getLineSpacing();
    sf::Transform windowToStage = stage.WindowToStage();
    sf::Vector2f textSizeInWindow(localEndPos.x, localEndPos.y + lineSpacing);
    float windowToStageXScale = windowToStage.getMatrix()[0];
    float windowToStageYScale = windowToStage.getMatrix()[5];
    return sf::Vector2f(windowToStageXScale * textSizeInWindow.x, windowToStageYScale * textSizeInWindow.y);
}

void DrawText(sf::RenderWindow &window, Stage &stage, sf::Text &text, sf::FloatRect bounds, TextAlignment alignment)
{
    sf::Vector2f textSizeInStage = MeasureText(stage, text);
    sf::Vector2f textPositionInStage;
    textPositionInStage.y = bounds.top;
    textPositionInStage.x = bounds.left;
    if (alignment == TextAlignment::Right)
    {
        textPositionInStage.x = bounds.left + bounds.width - textSizeInStage.x;
    }
    else if (alignment == TextAlignment::Center)
    {
        textPositionInStage.x = bounds.left + 0.5f * (bounds.width - textSizeInStage.x);
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