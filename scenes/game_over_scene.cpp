#include "scenes/game_over_scene.h"

#include "scenes/leaderboard_scene.h"
#include "ui/text.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

int maxPlayerNameLength = 32;

class GameOverScene : public Scene
{
  public:
    GameOverScene(Game &game, int scoreInSeconds) : _game(game), _stage(game.GetStage()), _scoreInSeconds(scoreInSeconds)
    {
        _backgroundTexture.loadFromFile("assets/game_over_bg.jpeg");
        _background.setTexture(_backgroundTexture);
        _font.loadFromFile("assets/RobotoMono-VariableFont_wght.ttf");
        _gameOverText.setString("Game Over");
        _labelText.setString("Please enter your name:");
        _gameOverText.setFont(_font);
        _gameOverText.setFillColor(sf::Color::White);
        _gameOverText.setOutlineColor(sf::Color::Black);
        _gameOverText.setOutlineThickness(5);
        _gameOverText.setCharacterSize(200);
        for (sf::Text *text : {&_labelText, &_inputText})
        {
            text->setFont(_font);
            text->setFillColor(sf::Color::White);
            text->setOutlineColor(sf::Color::Black);
            text->setOutlineThickness(2);
            text->setCharacterSize(32);
        }
        _inputText.setFillColor(sf::Color::Yellow);
    }

    void Draw(sf::RenderWindow &window) override
    {
        sf::FloatRect stageBounds = _stage.GetBounds();
        _background.setPosition(stageBounds.left, stageBounds.top);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        _background.setScale(stageBounds.width / textureSize.x, stageBounds.height / textureSize.y);
        window.draw(_background);

        _inputText.setString(_playerNameInput);
        float gameOverHeight = MeasureText(_stage, _gameOverText).y;
        float topPadding = std::max(0.0f, 0.5f * stageBounds.height - gameOverHeight);
        float bottomPadding = 0.1f * stageBounds.height;
        float spaceRemaining = stageBounds.height - topPadding - gameOverHeight - bottomPadding;
        float labelHeight = MeasureText(_stage, _labelText).y;
        float lineSpacing = 1.2;
        sf::FloatRect bounds(stageBounds.left, stageBounds.top + topPadding, stageBounds.width,
                             stageBounds.height);
        DrawText(window, _stage, _gameOverText, bounds, TextAlignment::Center);
        bounds.top += gameOverHeight + std::max(0.1f * gameOverHeight, 0.5f * (spaceRemaining - (2 + lineSpacing) * labelHeight));
        DrawText(window, _stage, _labelText, bounds, TextAlignment::Center);
        bounds.top += lineSpacing * labelHeight;
        DrawText(window, _stage, _inputText, bounds, TextAlignment::Center);
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Enter)
        {
            ToLeaderboard();
        }
        if (event.code == sf::Keyboard::BackSpace)
        {
            if (!_playerNameInput.empty())
            {
                _playerNameInput.pop_back();
            }
        }
    }
    void OnTextEntered(sf::Event::TextEvent &event) override
    {
        if (_playerNameInput.size() >= maxPlayerNameLength)
        {
            // Reached player name length limit
            return;
        }
        if (event.unicode >= 128 || event.unicode == 8 /* backspace */)
        {
            // Ignore non-ASCII
            return;
        }
        char asciiChar = (char)event.unicode;
        _playerNameInput.push_back(asciiChar);
    }

  private:
    void ToLeaderboard()
    {
        std::vector<PlayerScore> scores = SubmitScore(_playerNameInput, _scoreInSeconds);
        _game.SetScene(CreateLeaderboardScene(_game, std::move(scores)));
    }

    Game &_game;
    Stage &_stage;
    int _scoreInSeconds;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
    std::string _playerNameInput;
    sf::Font _font;
    sf::Text _gameOverText;
    sf::Text _labelText;
    sf::Text _inputText;
};

std::unique_ptr<Scene> CreateGameOverScene(Game &game, int scoreInSeconds)
{
    return std::make_unique<GameOverScene>(game, scoreInSeconds);
}