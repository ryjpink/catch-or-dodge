#pragma once

#include "stage.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>

enum TextAlignment
{
    Left,
    Center,
    Right,
};

sf::FloatRect MeasureText(Stage &stage, sf::Text &text);

void DrawText(sf::RenderWindow &window, Stage &stage, sf::Text &text, sf::FloatRect bounds,
              TextAlignment alignment = TextAlignment::Left);