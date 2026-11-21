#include "GameStatePlaying.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include "DurableBrick.h"
#include "GlassBrick.h"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <string>

namespace SnakeGame
{
    namespace
    {
        const float BRICK_WIDTH = 68.f;
        const float BRICK_HEIGHT = 22.f;
        const float BRICK_GAP = 5.f;
        const int BRICK_COLUMNS = 10;
        const int BRICK_ROWS = 5;
        const float PI = 3.14159265f;

        void CreateLevel(GameStatePlayingData& data)
        {
            data.GetBricks().clear();

            const float totalWidth =
                BRICK_COLUMNS * BRICK_WIDTH + (BRICK_COLUMNS - 1) * BRICK_GAP;
            const float startX =
                (SCREEN_WIDTH - totalWidth) / 2.f + BRICK_WIDTH / 2.f;

            const std::vector<sf::Color> durableDamageColors = {
                sf::Color(170, 110, 230),
                sf::Color(235, 165, 65),
                sf::Color(235, 75, 75)
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
                        data.GetBricks().push_back(std::unique_ptr<Brick>(
                            new DurableBrick(data.GetBlockTexture(), position, size, durableDamageColors)));
                    else if (row == 4 && col % 4 == 0)
                        data.GetBricks().push_back(std::unique_ptr<Brick>(
                            new GlassBrick(data.GetBlockTexture(), position, size)));
                    else
                        data.GetBricks().push_back(std::unique_ptr<Brick>(
                            new Brick(data.GetBlockTexture(), position, size)));
                }
            }
        }

        void ReflectBallFromBrick(Ball& ball, const sf::FloatRect& brickBounds)
        {
            const sf::FloatRect ballBounds = ball.GetGlobalBounds();
            const float overlapLeft = ballBounds.left + ballBounds.width - brickBounds.left;
            const float overlapRight = brickBounds.left + brickBounds.width - ballBounds.left;
            const float overlapTop = ballBounds.top + ballBounds.height - brickBounds.top;
            const float overlapBottom = brickBounds.top + brickBounds.height - ballBounds.top;
            const float overlapX = std::min(overlapLeft, overlapRight);
            const float overlapY = std::min(overlapTop, overlapBottom);

            sf::Vector2f velocity = ball.GetVelocity();
            if (overlapX < overlapY) velocity.x = -velocity.x;
            else velocity.y = -velocity.y;
            ball.SetVelocity(velocity);
        }

        void ResetBallAndPlatform(GameStatePlayingData& data)
        {
            data.GetPlatform().SetWidth(PLATFORM_WIDTH);
            data.GetPlatform().Move(SCREEN_WIDTH / 2.f - data.GetPlatform().GetPosition().x,
                                    static_cast<float>(SCREEN_WIDTH));
            data.GetBall().SetPosition({ SCREEN_WIDTH / 2.f, SCREEN_HEGHT / 2.f });
            const float angle = 45.f * PI / 180.f;
            data.GetBall().SetVelocity({ std::cos(angle) * BALL_SPEED, -std::sin(angle) * BALL_SPEED });
        }

        GameMemento CaptureGame(const GameStatePlayingData& data)
        {
            GameMemento state;
            state.score = data.GetScore();
            state.lives = data.GetLives();
            state.ballX = data.GetBall().GetPosition().x;
            state.ballY = data.GetBall().GetPosition().y;
            state.velocityX = data.GetBall().GetVelocity().x;
            state.velocityY = data.GetBall().GetVelocity().y;
            state.platformX = data.GetPlatform().GetPosition().x;
            state.platformWidth = data.GetPlatform().GetWidth();
            for (const auto& brick : data.GetBricks())
            {
                BrickSnapshot snapshot;
                snapshot.type = brick->GetTypeId();
                snapshot.hitPoints = brick->GetHitPoints();
                snapshot.destroyed = brick->IsDestroyed();
                state.bricks.push_back(snapshot);
            }
            return state;
        }

        bool RestoreGame(GameStatePlayingData& data, const GameMemento& state)
        {
            if (state.bricks.size() != data.GetBricks().size()) return false;
            for (std::size_t i = 0; i < data.GetBricks().size(); ++i)
                if (data.GetBricks()[i]->GetTypeId() != state.bricks[i].type)
                    return false;

            data.GetBonusManager().Clear();
            data.GetScoreSubject() = ScoreSubject(state.score);
            data.SetLives(state.lives);
            data.GetBall().SetPosition({ state.ballX, state.ballY });
            data.GetBall().SetVelocity({ state.velocityX, state.velocityY });
            data.GetPlatform().SetWidth(state.platformWidth);
            data.GetPlatform().Move(state.platformX - data.GetPlatform().GetPosition().x,
                               static_cast<float>(SCREEN_WIDTH));

            for (std::size_t i = 0; i < data.GetBricks().size(); ++i)
            {
                data.GetBricks()[i]->RestoreState(state.bricks[i].destroyed,
                                             state.bricks[i].hitPoints);
            }
            data.GetScoreObserver().SetText(&data.GetScoreTextForObserver());
            data.GetScoreSubject().Attach(&data.GetScoreObserver());
            data.GetScoreObserver().OnScoreChanged(data.GetScoreSubject().GetScore());
            return true;
        }

        int CountRemainingBricks(const GameStatePlayingData& data)
        {
            int count = 0;
            for (const auto& brick : data.GetBricks())
                if (!brick->IsDestroyed()) ++count;
            return count;
        }
    }

    void InitGameStatePlaying(GameStatePlayingData& data, Game& game)
    {
        assert(data.GetPlatformTexture().loadFromFile(TEXTURES_PATH + "Platform.png"));
        assert(data.GetBallTexture().loadFromFile(TEXTURES_PATH + "Ball.png"));
        assert(data.GetBlockTexture().loadFromFile(TEXTURES_PATH + "Block.png"));
        assert(data.GetFont().loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        data.GetBackground().setSize({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEGHT) });
        data.GetBackground().setPosition(0.f, 0.f);
        data.GetBackground().setFillColor(sf::Color(30, 30, 30));

        data.GetPlatform() = Platform(data.GetPlatformTexture(), PLATFORM_WIDTH, PLATFORM_HEIGHT,
                                      { SCREEN_WIDTH / 2.f, SCREEN_HEGHT - PLATFORM_HEIGHT });
        data.GetBall() = Ball(data.GetBallTexture(), BALL_RADIUS,
                              { SCREEN_WIDTH / 2.f, SCREEN_HEGHT / 2.f });

        const float angle = 45.f * PI / 180.f;
        data.GetBall().SetVelocity({ std::cos(angle) * BALL_SPEED, -std::sin(angle) * BALL_SPEED });

        data.scoreSubject = ScoreSubject();
        data.SetLives(3);
        data.GetBonusManager().Clear();
        CreateLevel(data);

        data.GetScoreText().setFont(data.GetFont());
        data.GetScoreText().setCharacterSize(24);
        data.GetScoreText().setFillColor(sf::Color::Yellow);
        data.GetScoreObserver().SetText(&data.GetScoreTextForObserver());
        data.GetScoreSubject().Attach(&data.GetScoreObserver());
        data.GetScoreObserver().OnScoreChanged(0);

        data.GetLivesText().setFont(data.GetFont());
        data.GetLivesText().setCharacterSize(24);
        data.GetLivesText().setFillColor(sf::Color::Cyan);
        data.GetLivesText().setString("Lives: 3");

        data.GetInputHintText().setFont(data.GetFont());
        data.GetInputHintText().setCharacterSize(18);
        data.GetInputHintText().setFillColor(sf::Color::White);
        data.GetInputHintText().setString("Move: Left/Right or A/D | ESC: pause | F5: save | F9: load");
        data.GetInputHintText().setOrigin(GetTextOrigin(data.GetInputHintText(), { 1.f, 0.f }));
    }

    void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game)
    {
        data.GetScoreSubject().Detach(&data.GetScoreObserver());
    }

    void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed) return;

        if (event.key.code == sf::Keyboard::Escape)
            PushGameState(game, GameStateType::ExitDialog, false);
        else if (event.key.code == sf::Keyboard::F5)
            data.GetSaveManager().Save(CaptureGame(data));
        else if (event.key.code == sf::Keyboard::F9)
        {
            GameMemento saved;
            if (data.GetSaveManager().Load(saved))
                RestoreGame(data, saved);
        }
    }

    void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float timeDelta)
    {
        Platform& platform = data.GetPlatform();
        Ball& ball = data.GetBall();

        const BonusContext& bonus = data.GetBonusManager().GetContext();
        float shift = 0.f;
        const float platformSpeed = PLATFORM_SPEED * bonus.platformSpeedMultiplier;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            shift -= platformSpeed * timeDelta;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            shift += platformSpeed * timeDelta;

        platform.Move(shift, static_cast<float>(SCREEN_WIDTH));
        ball.Move(timeDelta);

        sf::Vector2f velocity = ball.GetVelocity();
        sf::FloatRect ballBounds = ball.GetGlobalBounds();

        if (ballBounds.left < 0.f) velocity.x = std::abs(velocity.x);
        else if (ballBounds.left + ballBounds.width > SCREEN_WIDTH) velocity.x = -std::abs(velocity.x);
        if (ballBounds.top < 0.f) velocity.y = std::abs(velocity.y);

        if (velocity.y > 0.f && ballBounds.intersects(platform.GetGlobalBounds()))
            velocity.y = -std::abs(velocity.y);

        ball.SetVelocity(velocity);

        for (auto& brick : data.GetBricks())
        {
            if (!brick->IsDestroyed() && ball.GetGlobalBounds().intersects(brick->GetGlobalBounds()))
            {
                const sf::FloatRect hitBounds = brick->GetGlobalBounds();
                const bool wasDestroyed = brick->IsDestroyed();

                if (bonus.fireball || bonus.fragileBlocks)
                {
                    brick->Destroy();
                    if (brick->GetTypeId() != 2)
                        ReflectBallFromBrick(ball, hitBounds);
                }
                else
                {
                    const bool shouldBounce = brick->OnHit();
                    if (shouldBounce) ReflectBallFromBrick(ball, hitBounds);
                }

                if (!wasDestroyed && brick->IsDestroyed())
                {
                    data.IncrementScore(10);
                    data.GetBonusManager().TrySpawn({ hitBounds.left + hitBounds.width / 2.f,
                                                 hitBounds.top + hitBounds.height / 2.f });
                }
                break;
            }
        }

        // Применяем ускорение мяча и размер платформы от активного бонуса.
        const float targetWidth = PLATFORM_WIDTH * bonus.platformWidthMultiplier;
        if (std::abs(platform.GetWidth() - targetWidth) > 0.5f)
            platform.SetWidth(targetWidth);

        const float currentSpeed = std::sqrt(ball.GetVelocity().x * ball.GetVelocity().x +
                                              ball.GetVelocity().y * ball.GetVelocity().y);
        const float targetSpeed = BALL_SPEED * bonus.ballSpeedMultiplier;
        if (currentSpeed > 0.f && std::abs(currentSpeed - targetSpeed) > 2.f)
        {
            sf::Vector2f v = ball.GetVelocity();
            ball.SetVelocity(v * (targetSpeed / currentSpeed));
        }

        data.GetBonusManager().Update(timeDelta, platform.GetGlobalBounds(), static_cast<float>(SCREEN_HEGHT));

        if (ball.GetPosition().y - BALL_RADIUS > SCREEN_HEGHT)
        {
            data.SetLives(data.GetLives() - 1);
            data.GetBonusManager().Clear();
            platform.SetWidth(PLATFORM_WIDTH);

            if (data.GetLives() <= 0)
            {
                game.recordsTable[PLAYER_NAME] =
                    std::max(game.recordsTable[PLAYER_NAME], data.GetScore());
                SwitchGameState(game, GameStateType::GameOver);
                return;
            }
            ResetBallAndPlatform(data);
        }

        if (CountRemainingBricks(data) == 0)
        {
            game.recordsTable[PLAYER_NAME] =
                std::max(game.recordsTable[PLAYER_NAME], data.GetScore());
            SwitchGameState(game, GameStateType::Win);
            return;
        }

        data.GetLivesText().setString("Lives: " + std::to_string(data.GetLives()));
    }

    void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window)
    {
        window.draw(data.GetBackground());
        for (const auto& brick : data.GetBricks()) brick->Draw(window);


        data.GetBonusManager().Draw(window);

        data.GetPlatform().Draw(window);
        data.GetBall().Draw(window);

        data.GetScoreText().setOrigin(GetTextOrigin(data.GetScoreText(), { 0.f, 0.f }));
        data.GetScoreText().setPosition(10.f, 10.f);
        window.draw(data.GetScoreText());

        data.GetLivesText().setOrigin(GetTextOrigin(data.GetLivesText(), { 0.f, 0.f }));
        data.GetLivesText().setPosition(10.f, 42.f);
        window.draw(data.GetLivesText());

        const sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();
        data.GetInputHintText().setPosition(viewSize.x - 10.f, 10.f);
        window.draw(data.GetInputHintText());
    }
}
