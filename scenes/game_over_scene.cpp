#include "scenes/game_over_scene.h"
#include "scenes/leaderboard_scene.h"

class GameOverScene : public Scene
{
  public:
    GameOverScene(Game &game) : _game(game), _stage(game.GetStage())
    {
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
};

std::unique_ptr<Scene> CreateGameOverScene(Game& game)
{
    return std::make_unique<GameOverScene>(game);
}