#include "play_scene.h"

#include "entities/ball.h"
#include "entities/bomb.h"
#include "entities/player.h"
#include "entity.h"
#include "life_bar.h"
#include "scenes/game_over_scene.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

#include <box2d/b2_world_callbacks.h>
#include <box2d/box2d.h>

#include <fmt/core.h>

#include <random>

float ballSpawnInterval = 2.5; // s
float bombSpawnInterval = 5;
float lifeLostPerSecond = 2;

// Gravity on Earth is 9.8067 m/s^2. Point it downwards (Y).
b2Vec2 gravity(0.0f, 9.8067);

// Options controlling the accuracy of the physics simulation
const int32 velocityIterations = 8;
const int32 positionIterations = 3;

const unsigned seed = std::random_device()();

void DrawInWindowView(sf::RenderWindow &window, sf::Drawable &drawable)
{
    sf::Vector2u windowSize = window.getSize();
    sf::View windowView(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    sf::View previousView = window.getView();
    window.setView(windowView);
    window.draw(drawable);
    window.setView(previousView);
}

class PlayScene : public Scene, b2ContactListener
{
  public:
    PlayScene(Game &game) : _game(game), _stage(game.GetStage())
    {
        // Create the player
        _player = std::make_shared<Player>(_world);
        sf::FloatRect stageBounds = _stage.GetBounds();
        b2Vec2 bottomCenter(stageBounds.left + 0.5 * stageBounds.width, stageBounds.top + stageBounds.height);
        _player->SetPosition(bottomCenter);
        entities.push_back(_player);

        _font.loadFromFile("assets/RobotoMono-VariableFont_wght.ttf");

        // Register the collision event handler
        _world.SetContactListener(this);
    }

    void Update(float timeStep) override
    {
        _lifeBar.Add(-lifeLostPerSecond * timeStep);
        if (_lifeBar.IsDead())
        {
            _game.SetScene(CreateGameOverScene(_game));
            return;
        }

        timeUntilNextBall -= timeStep;
        timeUntilNextBomb -= timeStep;
        while (timeUntilNextBall < 0)
        {
            SpawnBall();
            timeUntilNextBall += ballSpawnInterval;
        }

        while (timeUntilNextBomb < 0)
        {
            SpawnBomb();
            timeUntilNextBomb += bombSpawnInterval;
        }

        for (const std::shared_ptr<Entity> &entity : entities)
        {
            entity->Update();
        }

        _world.Step(timeStep, velocityIterations, positionIterations);

        CleanupDestroyedEntities();
    }

    void Draw(sf::RenderWindow &window) override
    {
        sf::RectangleShape background;
        sf::FloatRect stageBounds = _stage.GetBounds();
        background.setPosition(stageBounds.left, stageBounds.top);
        background.setSize(_stage.GetSize());
        background.setFillColor(sf::Color::White);
        window.draw(background);

        for (const std::shared_ptr<Entity> &entity : entities)
        {
            entity->Draw(window);
        }

        sf::RectangleShape lifeBarShape;
        float lifeBarHeight = 0.01f * stageBounds.height;
        lifeBarShape.setPosition(stageBounds.left, stageBounds.top);
        lifeBarShape.setSize(sf::Vector2f(stageBounds.width * _lifeBar.GetRatio(), lifeBarHeight));
        lifeBarShape.setFillColor(sf::Color::Red);
        window.draw(lifeBarShape);

        float seconds = _scoreTimer.getElapsedTime().asSeconds();
        int minutes = (int)(seconds / 60);
        seconds -= minutes * 60;

        std::string scoreString = fmt::format("Score: {:02}:{:02}", minutes, (int)seconds);
        sf::Text scoreText;
        scoreText.setFont(_font);
        scoreText.setString(scoreString);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setCharacterSize(32);
        sf::FloatRect textBoundsInStage = _stage.WindowToStage(scoreText.getLocalBounds());
        sf::Vector2f textPositionInStage(stageBounds.left + 0.99 * stageBounds.width - textBoundsInStage.width,
                                         stageBounds.top + 0.02 * stageBounds.height);
        sf::Vector2f textPositionInWindow = _stage.StageToWindow(textPositionInStage);
        scoreText.setPosition(textPositionInWindow);
        DrawInWindowView(window, scoreText);
    }

    void OnKeyPressed(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Left)
        {
            _player->SetMovingLeft(true);
        }
        if (event.code == sf::Keyboard::Right)
        {
            _player->SetMovingRight(true);
        }
    }

    void OnKeyReleased(sf::Event::KeyEvent &event) override
    {
        if (event.code == sf::Keyboard::Left)
        {
            _player->SetMovingLeft(false);
        }
        if (event.code == sf::Keyboard::Right)
        {
            _player->SetMovingRight(false);
        }
    }

  private:
    b2Vec2 GenerateRandomSpawnPosition()
    {
        sf::FloatRect stageBounds = _stage.GetBounds();
        std::uniform_real_distribution<float> xDistribution(stageBounds.left, stageBounds.left + stageBounds.width);
        return b2Vec2(xDistribution(_randomNumberGenerator), stageBounds.top);
    }

    void SpawnBall()
    {
        std::shared_ptr<Ball> ball = std::make_shared<Ball>(_world);
        ball->SetPosition(GenerateRandomSpawnPosition());
        entities.push_back(ball);
    }

    void SpawnBomb()
    {
        std::shared_ptr<Bomb> bomb = std::make_shared<Bomb>(_world);
        bomb->SetPosition(GenerateRandomSpawnPosition());
        entities.push_back(bomb);
    }

    void CleanupDestroyedEntities()
    {
        int remainingEntities = 0;
        for (int i = 0; i < entities.size(); ++i)
        {
            if (!entities[i]->IsDestroyed())
            {
                entities[remainingEntities] = entities[i];
                remainingEntities += 1;
            }
        }
        entities.resize(remainingEntities);
    }

    // Called when two fixtures begin to touch.
    void BeginContact(b2Contact *contact) override
    {
        // Get the two bodies involved in the collision
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();

        // Get the entities that the bodies represent
        Entity *entityA = (Entity *)bodyA->GetUserData().pointer;
        Entity *entityB = (Entity *)bodyB->GetUserData().pointer;
        if (entityA->IsDestroyed() || entityB->IsDestroyed())
        {
            return;
        }
        if (entityA->GetType() > entityB->GetType())
        {
            std::swap(entityA, entityB);
        }

        if (entityA->GetType() == Entity::Type::Player && entityB->GetType() == Entity::Type::Ball)
        {
            Player *player = (Player *)entityA;
            Ball *ball = (Ball *)entityB;
            _lifeBar.Add(20);
            ball->Destroy();
        }
        if (entityA->GetType() == Entity::Type::Player && entityB->GetType() == Entity::Type::Bomb)
        {
            Player *player = (Player *)entityA;
            Bomb *bomb = (Bomb *)entityB;
            _lifeBar.Add(-20);
            bomb->Destroy();
        }
    }

    // Called when two fixtures cease to touch.
    void EndContact(b2Contact *contact) override
    {
    }

    Game &_game;
    Stage &_stage;

    std::mt19937 _randomNumberGenerator = std::mt19937(seed);

    sf::Font _font;

    // Physics world
    b2World _world = b2World(gravity);

    std::vector<std::shared_ptr<Entity>> entities;

    std::shared_ptr<Player> _player;

    LifeBar _lifeBar;

    float timeUntilNextBall = 0;

    float timeUntilNextBomb = 2;

    sf::Clock _scoreTimer;
};

std::unique_ptr<Scene> CreatePlayScene(Game &game)
{
    return std::make_unique<PlayScene>(game);
}