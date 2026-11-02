#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

namespace SnakeGame
{
	class Game;

	class GameStatePauseMenuData
	{
	public:
		sf::Font& GetFont() { return font; }
		sf::RectangleShape& GetBackground() { return background; }
		sf::Text& GetTitleText() { return titleText; }
		Menu& GetMenu() { return menu; }
		MenuItem& GetResumeItem() { return resumeItem; }
		MenuItem& GetExitItem() { return exitItem; }

		friend void InitGameStatePauseMenu(GameStatePauseMenuData& data, Game& game);

	private:
		sf::Font font;
		sf::RectangleShape background;
		sf::Text titleText;
		Menu menu;
		MenuItem resumeItem;
		MenuItem exitItem;
	};

	void InitGameStatePauseMenu(GameStatePauseMenuData& data, Game& game);
	void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data, Game& game);
	void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, Game& game, const sf::Event& event);
	void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, float timeDelta);
	void DrawGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, sf::RenderWindow& window);
}