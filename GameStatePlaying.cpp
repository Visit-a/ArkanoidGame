#include "GameStatePlaying.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include "DurableBrick.h"
#include "GlassBrick.h"
#include <cassert>
#include <cmath>
#include <algorithm>

namespace SnakeGame
{
    namespace
    {
        const float BRICK_WIDTH = 68.f;
        const float BRICK_HEIGHT = 22.f;
        const float BRICK_GAP = 5.f;
        const int BRICK_COLUMNS = 10;
        const int BRICK_ROWS = 5;

        void CreateLevel(GameStatePlayingData& data)
        {
            data.GetBricks().clear();

            const float totalWidth =
                BRICK_COLUMNS * BRICK_WIDTH + (BRICK_COLUMNS - 1) * BRICK_GAP;
            const float startX =
                (SCREEN_WIDTH - totalWidth) / 2.f + BRICK_WIDTH / 2.f;

            // Оттенки применяются только к крепким блокам при получении урона.
            const std::vector<sf::Color> durableDamageColors = {
                sf::Color(170, 110, 230), // 3 HP
                sf::Color(235, 165, 65),  // 2 HP
                sf::Color(235, 75, 75)    // 1 HP
            };

            for (int row = 0; row < BRICK_ROWS; ++row)
            {
                for (int col = 0; col < BRICK_COLUMNS; ++col)
                {
                    const sf::Vector2f position(
                        startX + col * (BRICK_WIDTH + BRICK_GAP),
                        65.f + row * (BRICK_HEIGHT + BRICK_GAP));
                    const sf::Vector2f size(BRICK_WIDTH, BRICK_HEIGHT);

                    if ((row == 0 || row == 1) && col % 3 == 1)
                    {
                        data.GetBricks().push_back(
                            std::unique_ptr<Brick>(
                                new DurableBrick(data.GetBlockTexture(),
                                                 position, size,
                                                 durableDamageColors)));
                    }
                    else if (row == 4 && col % 4 == 0)
                    {
                        data.GetBricks().push_back(
                            std::unique_ptr<Brick>(
                                new GlassBrick(data.GetBlockTexture(), position, size)));
                    }
                    else
                    {
                        // Обычные блоки используют ту же текстуру без перекрашивания.
                        data.GetBricks().push_back(
                            std::unique_ptr<Brick>(
                                new Brick(data.GetBlockTexture(), position, size)));
                    }
                }
            }
        }

        void ReflectBallFromBrick(Ball& ball, const sf::FloatRect& brickBounds)
        {
            const sf::FloatRect ballBounds = ball.GetGlobalBounds();

            const float overlapLeft =
                ballBounds.left + ballBounds.width - brickBounds.left;
            const float overlapRight =
                brickBounds.left + brickBounds.width - ballBounds.left;
            const float overlapTop =
                ballBounds.top + ballBounds.height - brickBounds.top;
            const float overlapBottom =
                brickBounds.top + brickBounds.height - ballBounds.top;

            const float overlapX = std::min(overlapLeft, overlapRight);
            const float overlapY = std::min(overlapTop, overlapBottom);

            sf::Vector2f velocity = ball.GetVelocity();
            if (overlapX < overlapY)
                velocity.x = -velocity.x;
            else
                velocity.y = -velocity.y;

            ball.SetVelocity(velocity);
        }
    }

    void InitGameStatePlaying(GameStatePlayingData& data, Game& game)
    {
        assert(data.GetPlatformTexture().loadFromFile(TEXTURES_PATH + "Platform.png"));
        assert(data.GetBallTexture().loadFromFile(TEXTURES_PATH + "Ball.png"));
        assert(data.GetBlockTexture().loadFromFile(TEXTURES_PATH + "Block.png"));
        assert(data.GetFont().loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        data.GetBackground().setSize(
            sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEGHT));
        data.GetBackground().setPosition(0.f, 0.f);
        data.GetBackground().setFillColor(sf::Color(30, 30, 30));

        data.GetPlatform() = Platform(
            data.GetPlatformTexture(), PLATFORM_WIDTH, PLATFORM_HEIGHT,
            { SCREEN_WIDTH / 2.f, (float)SCREEN_HEGHT - PLATFORM_HEIGHT });

        data.GetBall() = Ball(
            data.GetBallTexture(), BALL_RADIUS,
            { SCREEN_WIDTH / 2.f, SCREEN_HEGHT / 2.f });

        const float angle = 45.f * 3.14159265f / 180.f;
        data.GetBall().SetVelocity({
            std::cos(angle) * BALL_SPEED,
            -std::sin(angle) * BALL_SPEED
        });

        data.score = 0;
        CreateLevel(data);

        data.GetScoreText().setFont(data.GetFont());
        data.GetScoreText().setCharacterSize(24);
        data.GetScoreText().setFillColor(sf::Color::Yellow);
        data.GetScoreText().setString("Score: 0");

        data.GetInputHintText().setFont(data.GetFont());
        data.GetInputHintText().setCharacterSize(24);
        data.GetInputHintText().setFillColor(sf::Color::White);
        data.GetInputHintText().setString("Move: Left/Right or A/D. ESC - pause");
        data.GetInputHintText().setOrigin(
            GetTextOrigin(data.GetInputHintText(), { 1.f, 0.f }));
    }

    void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game) {}

    void HandleGameStatePlayingWindowEvent(
        GameStatePlayingData& data, Game& game, const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            PushGameState(game, GameStateType::ExitDialog, false);
    }

    void UpdateGameStatePlaying(
        GameStatePlayingData& data, Game& game, float timeDelta)
    {
        Platform& platform = data.GetPlatform();
        Ball& ball = data.GetBall();
        sf::Vector2f velocity = ball.GetVelocity();

        float shift = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            shift -= PLATFORM_SPEED * timeDelta;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            shift += PLATFORM_SPEED * timeDelta;

        platform.Move(shift, (float)SCREEN_WIDTH);
        ball.Move(timeDelta);

        sf::FloatRect ballBounds = ball.GetGlobalBounds();

        if (ballBounds.left < 0.f)
            velocity.x = std::abs(velocity.x);
        else if (ballBounds.left + ballBounds.width > SCREEN_WIDTH)
            velocity.x = -std::abs(velocity.x);

        if (ballBounds.top < 0.f)
            velocity.y = std::abs(velocity.y);

        if (velocity.y > 0.f &&
            ballBounds.intersects(platform.GetSprite().getGlobalBounds()))
            velocity.y = -std::abs(velocity.y);

        ball.SetVelocity(velocity);

        // Проверяем одно столкновение за кадр.
        for (auto& brick : data.GetBricks())
        {
            if (!brick->IsDestroyed() &&
                ball.GetGlobalBounds().intersects(brick->GetGlobalBounds()))
            {
                const sf::FloatRect hitBounds = brick->GetGlobalBounds();
                const bool shouldBounce = brick->OnHit();

                if (shouldBounce)
                    ReflectBallFromBrick(ball, hitBounds);

                if (brick->IsDestroyed())
                    data.IncrementScore();

                break;
            }
        }

        if (ball.GetPosition().y - BALL_RADIUS > SCREEN_HEGHT)
        {
            game.recordsTable[PLAYER_NAME] =
                std::max(game.recordsTable[PLAYER_NAME], data.GetScore());
            SwitchGameState(game, GameStateType::GameOver);
            return;
        }

        data.GetScoreText().setString(
            "Score: " + std::to_string(data.GetScore()));
    }

    void DrawGameStatePlaying(
        GameStatePlayingData& data, Game& game, sf::RenderWindow& window)
    {
        window.draw(data.GetBackground());

        for (const auto& brick : data.GetBricks())
            brick->Draw(window);

        data.GetPlatform().Draw(window);
        data.GetBall().Draw(window);

        data.GetScoreText().setOrigin(
            GetTextOrigin(data.GetScoreText(), { 0.f, 0.f }));
        data.GetScoreText().setPosition(10.f, 10.f);
        window.draw(data.GetScoreText());

        const sf::Vector2f viewSize =
            (sf::Vector2f)window.getView().getSize();
        data.GetInputHintText().setPosition(viewSize.x - 10.f, 10.f);
        window.draw(data.GetInputHintText());
    }
}
