#pragma once

#include "stage.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>
#include <string>

class Table
{
  public:
    Table(std::vector<std::string> headers);

    void AddRow(std::vector<std::string> row);
    void Draw(Stage &stage, sf::FloatRect containerBounds, sf::RenderWindow &window);

  private:
    // Row 0 is the header, rows 1-N are the data.
    std::vector<std::vector<std::string>> _rows;
    sf::Font _font;
};