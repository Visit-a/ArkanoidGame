#include "GameStatePlaying.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include <assert.h>
#include <cmath>
#include <algorithm>

namespace SnakeGame
{
	void InitGameStatePlaying(GameStatePlayingData& data, Game& game)
	{
		assert(data.GetPlatformTexture().loadFromFile(TEXTURES_PATH + "Platform.png"));
		assert(data.GetBallTexture().loadFromFile(TEXTURES_PATH + "Ball.png"));
		assert(data.GetFont().loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		data.GetBackground().setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEGHT));
		data.GetBackground().setPosition(0.f, 0.f);
		data.GetBackground().setFillColor(sf::Color(30, 30, 30));

		data.GetPlatform() = Platform(
			data.GetPlatformTexture(),
			PLATFORM_WIDTH,
			PLATFORM_HEIGHT,
			{ SCREEN_WIDTH / 2.f, (float)SCREEN_HEGHT - PLATFORM_HEIGHT });

		data.GetBall() = Ball(
			data.GetBallTexture(),
			BALL_RADIUS,
			{ SCREEN_WIDTH / 2.f, SCREEN_HEGHT / 2.f });

		const float angle = 45.f * 3.14159265f / 180.f;
		data.GetBall().SetVelocity({
			std::cos(angle) * BALL_SPEED,
			-std::sin(angle) * BALL_SPEED
			});

		data.GetScoreText().setFont(data.GetFont());
		data.GetScoreText().setCharacterSize(24);
		data.GetScoreText().setFillColor(sf::Color::Yellow);
		data.GetScoreText().setString("Score: 0");

		data.GetInputHintText().setFont(data.GetFont());
		data.GetInputHintText().setCharacterSize(24);
		data.GetInputHintText().setFillColor(sf::Color::White);
		data.GetInputHintText().setString("Move: Left/Right or A/D. ESC - pause");
		data.GetInputHintText().setOrigin(GetTextOrigin(data.GetInputHintText(), { 1.f, 0.f }));
	}

	void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game) {}

	void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			PushGameState(game, GameStateType::ExitDialog, false);
		}
	}

	void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float timeDelta)
	{
		Platform& platform = data.GetPlatform();
		Ball& ball = data.GetBall();
		sf::Vector2f velocity = ball.GetVelocity();

		float shift = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			shift -= PLATFORM_SPEED * timeDelta;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			shift += PLATFORM_SPEED * timeDelta;

		platform.Move(shift, (float)SCREEN_WIDTH);
		ball.Move(timeDelta);

		sf::FloatRect ballBounds = ball.GetGlobalBounds();

		if (ballBounds.left < 0.f) velocity.x = std::abs(velocity.x);
		else if (ballBounds.left + ballBounds.width > SCREEN_WIDTH) velocity.x = -std::abs(velocity.x);

		if (ballBounds.top < 0.f) velocity.y = std::abs(velocity.y);

		if (velocity.y > 0.f && ballBounds.intersects(platform.GetSprite().getGlobalBounds()))
		{
			velocity.y = -std::abs(velocity.y);
		}

		ball.SetVelocity(velocity);

		if (ball.GetPosition().y - BALL_RADIUS > SCREEN_HEGHT)
		{
			game.recordsTable[PLAYER_NAME] = std::max(game.recordsTable[PLAYER_NAME], data.GetScore());
			SwitchGameState(game, GameStateType::GameOver);
			return;
		}

		data.GetScoreText().setString("Score: " + std::to_string(data.GetScore()));
	}

	void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window)
	{
		window.draw(data.GetBackground());
		data.GetPlatform().Draw(window);
		data.GetBall().Draw(window);

		data.GetScoreText().setOrigin(GetTextOrigin(data.GetScoreText(), { 0.f, 0.f }));
		data.GetScoreText().setPosition(10.f, 10.f);
		window.draw(data.GetScoreText());

		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();
		data.GetInputHintText().setPosition(viewSize.x - 10.f, 10.f);
		window.draw(data.GetInputHintText());
	}
}