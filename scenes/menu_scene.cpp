#include "menu_scene.h"
#include "scenes/play_scene.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class MenuScene : public Scene
{
  public:
    MenuScene(Game &game) : _game(game), _stage(game.GetStage())
    {
        _backgroundTexture.loadFromFile("assets/menu_bg.jpeg");
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
    sf::Texture _backgroundTexture;
    sf::Sprite _background;
};

std::unique_ptr<Scene> CreateMenuScene(Game &game)
{
    return std::make_unique<MenuScene>(game);
}