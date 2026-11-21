#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Platform.h"
#include "Ball.h"
#include "Brick.h"
#include "ScoreObserver.h"
#include "GameSave.h"
#include "BonusSystem.h"

namespace SnakeGame
{
    class Game;

    class ScoreTextObserver : public IScoreObserver
    {
    public:
        void SetText(sf::Text* text) { scoreText = text; }
        void OnScoreChanged(int newScore) override
        {
            if (scoreText) scoreText->setString("Score: " + std::to_string(newScore));
        }
    private:
        sf::Text* scoreText = nullptr;
    };

    class GameStatePlayingData
    {
    public:
        sf::Texture& GetPlatformTexture() { return platformTexture; }
        sf::Texture& GetBallTexture() { return ballTexture; }
        sf::Texture& GetBlockTexture() { return blockTexture; }
        sf::Font& GetFont() { return font; }

        Platform& GetPlatform() { return platform; }
        const Platform& GetPlatform() const { return platform; }
        Ball& GetBall() { return ball; }
        const Ball& GetBall() const { return ball; }
        std::vector<std::unique_ptr<Brick>>& GetBricks() { return bricks; }
        const std::vector<std::unique_ptr<Brick>>& GetBricks() const { return bricks; }

        sf::Text& GetScoreText() { return scoreText; }
        sf::Text& GetLivesText() { return livesText; }
        sf::Text& GetInputHintText() { return inputHintText; }
        sf::RectangleShape& GetBackground() { return background; }

        int GetScore() const { return scoreSubject.GetScore(); }
        int GetLives() const { return lives; }
        void SetLives(int value) { lives = value; }
        ScoreSubject& GetScoreSubject() { return scoreSubject; }
        ScoreTextObserver& GetScoreObserver() { return scoreObserver; }
        sf::Text& GetScoreTextForObserver() { return scoreText; }
        BonusManager& GetBonusManager() { return bonusManager; }
        SaveManager& GetSaveManager() { return saveManager; }
        void IncrementScore(int points = 10) { scoreSubject.AddPoints(points); }

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
        ScoreSubject scoreSubject;
        ScoreTextObserver scoreObserver;
        int lives = 3;

        BonusManager bonusManager;
        SaveManager saveManager;

        sf::Text scoreText;
        sf::Text livesText;
        sf::Text inputHintText;
        sf::RectangleShape background;
    };

    void InitGameStatePlaying(GameStatePlayingData& data, Game& game);
    void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game);
    void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event);
    void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float timeDelta);
    void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window);
}
