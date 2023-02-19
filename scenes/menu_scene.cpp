#include "menu_scene.h"
#include "scenes/play_scene.h"

class MenuScene : public Scene
{
  public:
    MenuScene(Game &game) : _game(game), _stage(game.GetStage())
    {
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Enter)
        {
            NewGame();
        }
    }

  private:
    void NewGame()
    {
        _game.SetScene(CreatePlayScene(_game));
    }

    Game &_game;
    Stage &_stage;
};

std::unique_ptr<Scene> CreateMenuScene(Game& game)
{
    return std::make_unique<MenuScene>(game);
}