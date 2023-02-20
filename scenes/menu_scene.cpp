#include "menu_scene.h"

#include "leaderboard.h"
#include "scenes/leaderboard_scene.h"
#include "scenes/play_scene.h"
#include "ui/text.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

enum MenuEntry
{
    NewGame,
    Leaderboard,
    Quit,

    Count,
};

class MenuScene : public Scene
{
  public:
    MenuScene(Game &game) : _game(game), _stage(game.GetStage())
    {
        _backgroundTexture.loadFromFile("assets/menu_bg.jpeg");
        _background.setTexture(_backgroundTexture);
        _font.loadFromFile("assets/RobotoMono-VariableFont_wght.ttf");

        _labels[NewGame].setString("New Game");
        _labels[Leaderboard].setString("Leaderboard");
        _labels[Quit].setString("Quit");
        for (sf::Text &label : _labels)
        {
            label.setFont(_font);
            label.setFillColor(sf::Color::White);
            label.setOutlineColor(sf::Color::Black);
            label.setOutlineThickness(5);
            label.setCharacterSize(75);
        }
    }

    void Draw(sf::RenderWindow &window) override
    {
        sf::FloatRect stageBounds = _stage.GetBounds();
        _background.setPosition(stageBounds.left, stageBounds.top);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        _background.setScale(stageBounds.width / textureSize.x, stageBounds.height / textureSize.y);
        window.draw(_background);

        float menuHeight = 0;
        sf::Vector2f labelSizes[MenuEntry::Count];
        for (int i = 0; i < MenuEntry::Count; ++i)
        {
            sf::Text &label = _labels[i];
            labelSizes[i] = MeasureText(_stage, label);
            menuHeight += labelSizes[i].y;
        }

        float weightBefore = 5;
        float weightAfter = 1;
        float remainingSpace = std::max(0.0f, stageBounds.height - menuHeight);
        float spaceBefore = remainingSpace * weightBefore / (weightBefore + weightAfter);

        float currentY = stageBounds.top + spaceBefore;
        for (int i = 0; i < MenuEntry::Count; ++i)
        {
            sf::Text &label = _labels[i];
            label.setFillColor(_selectedEntry == i ? sf::Color::Yellow : sf::Color::White);
            float leftRightPadding = 0.05 * stageBounds.width;
            sf::FloatRect bounds(stageBounds.left + leftRightPadding, currentY,
                                 stageBounds.width - 2 * leftRightPadding, labelSizes[i].y);
            currentY += bounds.height;
            DrawText(window, _stage, label, bounds, TextAlignment::Left);
        }
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Enter)
        {
            if (_selectedEntry == MenuEntry::NewGame)
            {
                StartNewGame();
            }
            else if (_selectedEntry == MenuEntry::Leaderboard)
            {
                ToLeaderboard();
            }
            else if (_selectedEntry == MenuEntry::Quit)
            {
                std::exit(0);
            }
        }
        else if (event.code == sf::Keyboard::Up)
        {
            _selectedEntry = (_selectedEntry + MenuEntry::Count - 1) % MenuEntry::Count;
        }
        else if (event.code == sf::Keyboard::Down)
        {
            _selectedEntry = (_selectedEntry + 1) % MenuEntry::Count;
        }
    }

  private:
    void StartNewGame()
    {
        _game.SetScene(CreatePlayScene(_game));
    }

    void ToLeaderboard()
    {
        std::vector<PlayerScore> scores = GetHighscores();
        _game.SetScene(CreateLeaderboardScene(_game, std::move(scores)));
    }

    Game &_game;
    Stage &_stage;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
    sf::Font _font;

    sf::Text _labels[MenuEntry::Count];
    int _selectedEntry = MenuEntry::NewGame;
};

std::unique_ptr<Scene> CreateMenuScene(Game &game)
{
    return std::make_unique<MenuScene>(game);
}