#include "GameStatePlaying.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include <cassert>
#include <cmath>
#include <algorithm>

namespace SnakeGame
{
    namespace
    {
        void CreateBlocks(GameStatePlayingData& data)
        {
            data.GetBlocks().clear();

            const float gap = 5.f;
            const float totalWidth =
                BLOCK_COLUMNS * BLOCK_WIDTH + (BLOCK_COLUMNS - 1) * gap;
            const float startX =
                (SCREEN_WIDTH - totalWidth) / 2.f + BLOCK_WIDTH / 2.f;

            for (int row = 0; row < BLOCK_ROWS; ++row)
            {
                for (int col = 0; col < BLOCK_COLUMNS; ++col)
                {
                    const sf::Vector2f position(
                        startX + col * (BLOCK_WIDTH + gap),
                        65.f + row * (BLOCK_HEIGHT + gap));

                    data.GetBlocks().emplace_back(
                        data.GetBlockTexture(),
                        position,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
                }
            }
        }

        void ReflectBallFromBlock(Ball& ball, const sf::FloatRect& blockBounds)
        {
            const sf::FloatRect ballBounds = ball.GetGlobalBounds();

            const float overlapLeft =
                ballBounds.left + ballBounds.width - blockBounds.left;
            const float overlapRight =
                blockBounds.left + blockBounds.width - ballBounds.left;
            const float overlapTop =
                ballBounds.top + ballBounds.height - blockBounds.top;
            const float overlapBottom =
                blockBounds.top + blockBounds.height - ballBounds.top;

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
        data.GetBackground().setFillColor(sf::Color(15, 18, 35));

        data.GetPlatform() = Platform(
            data.GetPlatformTexture(),
            PLATFORM_WIDTH,
            PLATFORM_HEIGHT,
            { SCREEN_WIDTH / 2.f, (float)SCREEN_HEGHT - PLATFORM_HEIGHT });

        data.GetBall() = Ball(
            data.GetBallTexture(),
            BALL_RADIUS,
            { SCREEN_WIDTH / 2.f, SCREEN_HEGHT - 70.f });

        const float angle = 45.f * 3.14159265f / 180.f;
        data.GetBall().SetVelocity({
            std::cos(angle) * BALL_SPEED,
            -std::sin(angle) * BALL_SPEED
        });

        data.score = 0;
        CreateBlocks(data);

        data.GetScoreText().setFont(data.GetFont());
        data.GetScoreText().setCharacterSize(24);
        data.GetScoreText().setFillColor(sf::Color::Yellow);
        data.GetScoreText().setString("Score: 0");

        data.GetInputHintText().setFont(data.GetFont());
        data.GetInputHintText().setCharacterSize(18);
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
        {
            PushGameState(game, GameStateType::ExitDialog, false);
        }
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
        ball.Update(timeDelta);

        sf::FloatRect ballBounds = ball.GetGlobalBounds();

        if (ballBounds.left < 0.f)
        {
            velocity.x = std::abs(velocity.x);
            ball.SetPosition({ BALL_RADIUS, ball.GetPosition().y });
        }
        else if (ballBounds.left + ballBounds.width > SCREEN_WIDTH)
        {
            velocity.x = -std::abs(velocity.x);
            ball.SetPosition({ SCREEN_WIDTH - BALL_RADIUS, ball.GetPosition().y });
        }

        if (ballBounds.top < 0.f)
        {
            velocity.y = std::abs(velocity.y);
            ball.SetPosition({ ball.GetPosition().x, BALL_RADIUS });
        }

        if (velocity.y > 0.f &&
            ball.GetGlobalBounds().intersects(platform.GetGlobalBounds()))
        {
            velocity.y = -std::abs(velocity.y);

            const float offset =
                (ball.GetPosition().x - platform.GetPosition().x) /
                (PLATFORM_WIDTH / 2.f);

            const float clampedOffset =
                std::max(-1.f, std::min(1.f, offset));

            velocity.x = clampedOffset * BALL_SPEED * 0.8f;

            const float speed =
                std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            if (speed > 0.f)
                velocity *= BALL_SPEED / speed;
        }

        ball.SetVelocity(velocity);

        for (Block& block : data.GetBlocks())
        {
            if (block.IsActive() &&
                ball.GetGlobalBounds().intersects(block.GetGlobalBounds()))
            {
                ReflectBallFromBlock(ball, block.GetGlobalBounds());
                block.Destroy();
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

        const bool allDestroyed = std::none_of(
            data.GetBlocks().begin(),
            data.GetBlocks().end(),
            [](const Block& block) { return block.IsActive(); });

        if (allDestroyed)
        {
            game.recordsTable[PLAYER_NAME] =
                std::max(game.recordsTable[PLAYER_NAME], data.GetScore());
            SwitchGameState(game, GameStateType::Win);
            return;
        }

        data.GetScoreText().setString(
            "Score: " + std::to_string(data.GetScore()));
    }

    void DrawGameStatePlaying(
        GameStatePlayingData& data, Game& game, sf::RenderWindow& window)
    {
        window.draw(data.GetBackground());

        for (const Block& block : data.GetBlocks())
        {
            if (block.IsActive())
                block.Draw(window);
        }

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
