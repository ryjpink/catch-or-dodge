#include "scenes/leaderboard_scene.h"
#include "scenes/menu_scene.h"

class LeaderboardScene : public Scene
{
  public:
    LeaderboardScene(Game &game) : _game(game), _stage(game.GetStage())
    {
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
};

std::unique_ptr<Scene> CreateLeaderboardScene(Game& game)
{
    return std::make_unique<LeaderboardScene>(game);
}