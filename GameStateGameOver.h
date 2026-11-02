#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace SnakeGame
{
	class Game;

	class GameStateGameOverData
	{
	public:
		sf::Font& GetFont() { return font; }
		sf::RectangleShape& GetBackground() { return background; }
		sf::Text& GetGameOverText() { return gameOverText; }
		sf::Text& GetHintText() { return hintText; }
		std::vector<sf::Text>& GetRecordsTableTexts() { return recordsTableTexts; }
		float& GetTimeSinceGameOver() { return timeSinceGameOver; }

		friend void InitGameStateGameOver(GameStateGameOverData& data, Game& game);
		friend void UpdateGameStateGameOver(GameStateGameOverData& data, Game& game, float timeDelta);
		friend void DrawGameStateGameOver(GameStateGameOverData& data, Game& game, sf::RenderWindow& window);

	private:
		sf::Font font;
		float timeSinceGameOver = 0.f;
		sf::RectangleShape background;
		sf::Text gameOverText;
		sf::Text hintText;
		std::vector<sf::Text> recordsTableTexts;
	};

	void InitGameStateGameOver(GameStateGameOverData& data, Game& game);
	void ShutdownGameStateGameOver(GameStateGameOverData& data, Game& game);
	void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, Game& game, const sf::Event& event);
	void UpdateGameStateGameOver(GameStateGameOverData& data, Game& game, float timeDelta);
	void DrawGameStateGameOver(GameStateGameOverData& data, Game& game, sf::RenderWindow& window);
}