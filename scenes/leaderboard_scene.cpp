#include "scenes/leaderboard_scene.h"

#include "scenes/menu_scene.h"
#include "ui/table.h"
#include "ui/text.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <fmt/core.h>

class LeaderboardScene : public Scene
{
  public:
    LeaderboardScene(Game &game, std::vector<PlayerScore> scores)
        : _game(game), _stage(game.GetStage()), _table({"Rank", "Name", "Score"}), _scores(std::move(scores))
    {
        _backgroundMusic.openFromFile("assets/leaderboard_music.ogg");
        _backgroundMusic.setLoop(true);
        _backgroundMusic.play();

        _backgroundTexture.loadFromFile("assets/leaderboard_bg.jpeg");
        _background.setTexture(_backgroundTexture);
        _font.loadFromFile("assets/RobotoMono-VariableFont_wght.ttf");
        _titleText.setString("Leaderboard");
        _titleText.setFont(_font);
        _titleText.setFillColor(sf::Color::White);
        _titleText.setOutlineColor(sf::Color::Black);
        _titleText.setOutlineThickness(4);

        for (PlayerScore &score : _scores)
        {
            float seconds = score.scoreInSeconds;
            int minutes = (int)(seconds / 60);
            seconds -= minutes * 60;

            std::string scoreString = fmt::format("{:02}:{:02}", minutes, (int)seconds);
            _table.AddRow({std::to_string(score.rank), score.playerName, scoreString});
        }
    }

    void Draw(sf::RenderWindow &window) override
    {
        sf::FloatRect stageBounds = _stage.GetBounds();
        _background.setPosition(stageBounds.left, stageBounds.top);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        _background.setScale(stageBounds.width / textureSize.x, stageBounds.height / textureSize.y);
        window.draw(_background);

        float stageToWindowYScale = _stage.StageToWindow().getMatrix()[5];
        int textSizeInPixels = stageToWindowYScale * (stageBounds.height / 15.0f);
        _titleText.setCharacterSize(textSizeInPixels);
        float titleHeight = MeasureText(_stage, _titleText).y;
        float topPadding = std::max(0.0f, 0.15f * stageBounds.height - titleHeight);
        float spaceRemaining = stageBounds.height - topPadding - titleHeight;
        sf::FloatRect bounds(stageBounds.left, stageBounds.top + topPadding, stageBounds.width, stageBounds.height);
        DrawText(window, _stage, _titleText, bounds, TextAlignment::Center);
        bounds.top += 0.1 * titleHeight;
        bounds.height = stageBounds.height - (bounds.top - stageBounds.top);
        _table.Draw(_stage, bounds, window);
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Enter)
        {
            BackToMainMenu();
        }
    }

  private:
    void BackToMainMenu()
    {
        _backgroundMusic.stop();
        _game.SetScene(CreateMenuScene(_game));
    }

    Game &_game;
    Stage &_stage;
    sf::Music _backgroundMusic;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
    std::vector<PlayerScore> _scores;
    sf::Font _font;
    sf::Text _titleText;
    Table _table;
};

std::unique_ptr<Scene> CreateLeaderboardScene(Game &game, std::vector<PlayerScore> scores)
{
    return std::make_unique<LeaderboardScene>(game, std::move(scores));
}