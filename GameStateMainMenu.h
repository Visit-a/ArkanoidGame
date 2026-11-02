#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"

namespace SnakeGame
{
	class Game;

	class GameStateMainMenuData
	{
	public:
		sf::Font& GetFont() { return font; }
		Menu& GetMenu() { return menu; }
		MenuItem& GetStartGameItem() { return startGameItem; }
		MenuItem& GetOptionsItem() { return optionsItem; }
		MenuItem& GetOptionsInfiniteApplesItem() { return optionsInfiniteApplesItem; }
		MenuItem& GetOptionsWithAccelerationItem() { return optionsWithAccelerationItem; }
		MenuItem& GetRecordsItem() { return recordsItem; }
		MenuItem& GetExitGameItem() { return exitGameItem; }
		MenuItem& GetYesItem() { return yesItem; }
		MenuItem& GetNoItem() { return noItem; }

		friend void InitGameStateMainMenu(GameStateMainMenuData& data, Game& game);
		friend void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, Game& game, const sf::Event& event);
		friend void UpdateGameStateMainMenu(GameStateMainMenuData& data, Game& game, float timeDelta);
		friend void DrawGameStateMainMenu(GameStateMainMenuData& data, Game& game, sf::RenderWindow& window);

	private:
		sf::Font font;
		MenuItem startGameItem;
		MenuItem optionsItem;
		MenuItem optionsInfiniteApplesItem;
		MenuItem optionsWithAccelerationItem;
		MenuItem recordsItem;
		MenuItem exitGameItem;
		MenuItem yesItem;
		MenuItem noItem;
		Menu menu;
	};

	void InitGameStateMainMenu(GameStateMainMenuData& data, Game& game);
	void ShutdownGameStateMainMenu(GameStateMainMenuData& data, Game& game);
	void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, Game& game, const sf::Event& event);
	void UpdateGameStateMainMenu(GameStateMainMenuData& data, Game& game, float timeDelta);
	void DrawGameStateMainMenu(GameStateMainMenuData& data, Game& game, sf::RenderWindow& window);
}