#include "ball.h"
#include "player.h"
#include "life_bar.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <box2d/b2_world_callbacks.h>
#include <box2d/box2d.h>

#include <memory>
#include <random>

using namespace std::chrono_literals;

float stageAspectRatio = 16 / 9.0;
float stageWidth = 15;                             // m
float stageHeight = stageWidth / stageAspectRatio; // m

float ballSpawnInterval = 5; // s
float lifeLostPerSecond = 2;

// Gravity on Earth is 9.8067 m/s^2. Point it downwards (-Y).
b2Vec2 gravity(0.0f, -9.8067);

// Options controlling the accuracy of the physics simulation
const int32 velocityIterations = 8;
const int32 positionIterations = 3;

const unsigned seed = std::random_device()();

class Game : b2ContactListener
{
  public:
    Game()
    {
        // Create the player
        _player = std::make_shared<Player>(_world);
        entities.push_back(_player);

        // Register the collision event handler
        _world.SetContactListener(this);
    }

    void Update(float timeStep)
    {
        _lifeBar.Add(-lifeLostPerSecond * timeStep);
        if (_lifeBar.IsDead())
        {
            _player->SetMovingLeft(false);
            _player->SetMovingRight(false);
        }

        timeUntilNextBall -= timeStep;
        while (timeUntilNextBall < 0)
        {
            SpawnBall();
            timeUntilNextBall += ballSpawnInterval;
        }

        for (const std::shared_ptr<Entity> &entity : entities)
        {
            entity->Update();
        }

        _world.Step(timeStep, velocityIterations, positionIterations);

        CleanupDestroyedEntities();
    }

    void Draw(sf::RenderWindow &window)
    {
        for (const std::shared_ptr<Entity> &entity : entities)
        {
            entity->Draw(window);
        }

        sf::RectangleShape lifeBarShape;
        float lifeBarHeight = 0.01f * stageHeight;
        lifeBarShape.setPosition(-0.5f * stageWidth, stageHeight - lifeBarHeight);
        lifeBarShape.setSize(sf::Vector2f(stageWidth * _lifeBar.GetRatio(), lifeBarHeight));
        lifeBarShape.setFillColor(sf::Color::Red);
        window.draw(lifeBarShape);
    }

    void OnKeyPressed(sf::Event::KeyEvent &event)
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

    void OnKeyReleased(sf::Event::KeyEvent &event)
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
        std::uniform_real_distribution<float> xDistribution(-0.5 * stageWidth, 0.5 * stageWidth);
        return b2Vec2(xDistribution(_randomNumberGenerator), stageHeight);
    }

    void SpawnBall()
    {
        std::shared_ptr<Ball> ball = std::make_shared<Ball>(_world);
        ball->SetPosition(GenerateRandomSpawnPosition());
        entities.push_back(ball);
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
    void BeginContact(b2Contact *contact)
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
    }

    // Called when two fixtures cease to touch.
    void EndContact(b2Contact *contact)
    {
    }

    std::mt19937 _randomNumberGenerator = std::mt19937(seed);

    // Physics world
    b2World _world = b2World(gravity);

    std::vector<std::shared_ptr<Entity>> entities;

    std::shared_ptr<Player> _player;

    LifeBar _lifeBar;

    float timeUntilNextBall = 0;
};

void UpdateViewport(sf::RenderWindow &window)
{
    sf::Vector2u windowSize = window.getSize();

    float windowAspectRatio = windowSize.x / float(windowSize.y);
    float viewportWidth = stageWidth;
    float viewportHeight = stageHeight;
    if (windowAspectRatio > stageAspectRatio)
    {
        // Window wider than stage, letterbox left/right
        viewportWidth = stageHeight * windowAspectRatio;
    }
    else
    {
        // Stage wider than window, letterbox top/bottom
        viewportHeight = stageWidth / windowAspectRatio;
    }
    sf::View view;
    view.setSize(viewportWidth, -viewportHeight);
    view.setCenter(0, (stageHeight - viewportHeight) / 2 + viewportHeight / 2);
    window.setView(view);
}

int main()
{
    Game game;

    // Create a window.
    float initialWindowWidth = 1600;
    float initialWindowHeight = initialWindowWidth / stageAspectRatio;
    sf::RenderWindow window(sf::VideoMode(initialWindowWidth, initialWindowHeight), "Catch or Dodge");
    window.setVerticalSyncEnabled(true);

    // Create the stage.
    sf::RectangleShape stage;
    stage.setOrigin(sf::Vector2f(0.5 * stageWidth, 0));
    stage.setSize(sf::Vector2f(stageWidth, stageHeight));
    stage.setFillColor(sf::Color::White);

    // Set up the camera.
    UpdateViewport(window);

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle inputs
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                UpdateViewport(window);
            }
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                game.OnKeyPressed(event.key);
            }
            if (event.type == sf::Event::KeyReleased)
            {
                game.OnKeyReleased(event.key);
            }
        }

        float timeStep = clock.getElapsedTime().asSeconds();
        clock.restart();

        game.Update(timeStep);

        // Render the scene.
        window.clear(sf::Color::Black);
        window.draw(stage);
        game.Draw(window);
        window.display();
    }
}