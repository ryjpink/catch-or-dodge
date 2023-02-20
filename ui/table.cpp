#include "ui/table.h"

#include "ui/text.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

Table::Table(std::vector<std::string> headers)
{
    _font.loadFromFile("assets/RobotoMono-VariableFont_wght.ttf");
    _rows.push_back(headers);
}

void Table::AddRow(std::vector<std::string> row)
{
    _rows.push_back(row);
}

void Table::Draw(Stage &stage, sf::FloatRect containerBounds, sf::RenderWindow &window)
{
    sf::FloatRect stageBounds = stage.GetBounds();
    float stageToWindowYScale = stage.StageToWindow().getMatrix()[5];
    int textSizeInPixels = stageToWindowYScale * (stageBounds.height / 35.0f);
    std::vector<std::vector<sf::Text>> textRows;
    int numRows = _rows.size();
    int numColumns = _rows[0].size();
    std::vector<float> columnWidths(numColumns);
    float rowHeight = 0;
    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex)
    {
        std::vector<std::string> &row = _rows[rowIndex];
        std::vector<sf::Text> textRow;
        for (int columnIndex = 0; columnIndex < numColumns; ++columnIndex)
        {
            sf::Text text;
            text.setFont(_font);
            text.setFillColor(rowIndex == 0 ? sf::Color::Yellow : sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(2);
            text.setCharacterSize(textSizeInPixels);
            text.setString(row[columnIndex]);
            sf::Vector2f textSize = MeasureText(stage, text);
            columnWidths[columnIndex] = std::max(columnWidths[columnIndex], textSize.x);
            rowHeight = std::max(rowHeight, textSize.y);
            textRow.push_back(std::move(text));
        }
        textRows.push_back(std::move(textRow));
    }

    float cellPadding = 0.3 * rowHeight;
    rowHeight += 2 * cellPadding;
    for (int columnIndex = 0; columnIndex < numColumns; ++columnIndex)
    {
        columnWidths[columnIndex] += 2 * cellPadding;
    }

    float totalHeight = numRows * rowHeight;
    float totalWidth = 0;
    for (int columnIndex = 0; columnIndex < numColumns; ++columnIndex)
    {
        totalWidth += columnWidths[columnIndex];
    }

    float currentY = containerBounds.top + 0.5 * (containerBounds.height - totalHeight);
    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex)
    {
        std::vector<sf::Text> &textRow = textRows[rowIndex];
        float currentX = containerBounds.left + 0.5 * (containerBounds.width - totalWidth);
        for (int columnIndex = 0; columnIndex < numColumns; ++columnIndex)
        {
            sf::Text &text = textRow[columnIndex];
            float columnWidth = columnWidths[columnIndex];

            float outlineThickness = 0.04 * rowHeight;
            sf::RectangleShape cell;
            cell.setPosition(currentX, currentY);
            cell.setSize(sf::Vector2f(columnWidth, rowHeight));
            if (rowIndex == 0)
            {
                cell.setFillColor(sf::Color(0xf64a8afe));
            }
            else if (rowIndex % 2)
            {
                cell.setFillColor(sf::Color(0xffe5eafe));
            }
            else
            {
                cell.setFillColor(sf::Color(0xffc6d0fe));
            }
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(outlineThickness);
            window.draw(cell);

            float textX = currentX + cellPadding;
            float textY = currentY + cellPadding;
            sf::FloatRect cellBounds(textX, textY, rowHeight, columnWidth);
            DrawText(window, stage, text, cellBounds, TextAlignment::Left);
            currentX += columnWidth;
        }
        currentY += rowHeight;
    }
}
