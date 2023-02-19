#include "scenes/game_over_scene.h"
#include "scenes/leaderboard_scene.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class GameOverScene : public Scene
{
  public:
    GameOverScene(Game &game) : _game(game), _stage(game.GetStage())
    {
        _backgroundTexture.loadFromFile("assets/game_over_bg.jpeg");
        _background.setTexture(_backgroundTexture);
    }

    void Draw(sf::RenderWindow &window) override
    {
        sf::FloatRect stageBounds = _stage.GetBounds();
        _background.setPosition(stageBounds.left, stageBounds.top);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        _background.setScale(stageBounds.width / textureSize.x, stageBounds.height / textureSize.y);
        window.draw(_background);
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Enter)
        {
            ToLeaderboard();
        }
    }

  private:
    void ToLeaderboard()
    {
        _game.SetScene(CreateLeaderboardScene(_game));
    }

    Game &_game;
    Stage &_stage;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
};

std::unique_ptr<Scene> CreateGameOverScene(Game& game)
{
    return std::make_unique<GameOverScene>(game);
}