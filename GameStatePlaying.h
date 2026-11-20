#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Platform.h"
#include "Ball.h"
#include "Brick.h"

namespace SnakeGame
{
    class Game;

    class GameStatePlayingData
    {
    public:
        sf::Texture& GetPlatformTexture() { return platformTexture; }
        sf::Texture& GetBallTexture() { return ballTexture; }
        sf::Texture& GetBlockTexture() { return blockTexture; }
        sf::Font& GetFont() { return font; }

        Platform& GetPlatform() { return platform; }
        Ball& GetBall() { return ball; }
        std::vector<std::unique_ptr<Brick>>& GetBricks() { return bricks; }

        sf::Text& GetScoreText() { return scoreText; }
        sf::Text& GetInputHintText() { return inputHintText; }
        sf::RectangleShape& GetBackground() { return background; }

        int GetScore() const { return score; }
        void IncrementScore() { ++score; }

        friend void InitGameStatePlaying(GameStatePlayingData& data, Game& game);
        friend void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float timeDelta);
        friend void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window);

    private:
        sf::Texture platformTexture;
        sf::Texture ballTexture;
        sf::Texture blockTexture;
        sf::Font font;

        Platform platform;
        Ball ball;
        std::vector<std::unique_ptr<Brick>> bricks;
        int score = 0;

        sf::Text scoreText;
        sf::Text inputHintText;
        sf::RectangleShape background;
    };

    void InitGameStatePlaying(GameStatePlayingData& data, Game& game);
    void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game);
    void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event);
    void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float timeDelta);
    void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window);
}
