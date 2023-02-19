#include "scenes/leaderboard_scene.h"
#include "scenes/menu_scene.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class LeaderboardScene : public Scene
{
  public:
    LeaderboardScene(Game &game) : _game(game), _stage(game.GetStage())
    {
        _backgroundTexture.loadFromFile("assets/leaderboard_bg.jpeg");
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
            BackToMainMenu();
        }
    }

  private:
    void BackToMainMenu()
    {
        _game.SetScene(CreateMenuScene(_game));
    }

    Game &_game;
    Stage &_stage;
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
};

std::unique_ptr<Scene> CreateLeaderboardScene(Game& game)
{
    return std::make_unique<LeaderboardScene>(game);
}