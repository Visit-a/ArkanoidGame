#include "GameStatePauseMenu.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>

namespace SnakeGame
{
	void InitGameStatePauseMenu(GameStatePauseMenuData& data, Game& game)
	{
		assert(data.GetFont().loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		data.GetBackground().setFillColor(sf::Color(0, 0, 0, 128));

		data.GetTitleText().setString("Pause");
		data.GetTitleText().setFont(data.GetFont());
		data.GetTitleText().setCharacterSize(48);
		data.GetTitleText().setFillColor(sf::Color::Red);

		Menu& menu = data.GetMenu();
		MenuItem& root = menu.GetRootItem();
		root.SetChildrenOrientation(Orientation::Vertical);
		root.SetChildrenAlignment(Alignment::Middle);
		root.AddChild(&data.GetResumeItem());
		root.AddChild(&data.GetExitItem());

		data.GetResumeItem().GetText().setString("Return to game");
		data.GetResumeItem().GetText().setFont(data.GetFont());
		data.GetResumeItem().GetText().setCharacterSize(24);

		data.GetExitItem().GetText().setString("Exit to main menu");
		data.GetExitItem().GetText().setFont(data.GetFont());
		data.GetExitItem().GetText().setCharacterSize(24);

		InitMenuItem(menu.GetRootItem());
		SelectMenuItem(menu, &data.GetResumeItem());
	}

	void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data, Game& game) {}

	void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape) PopGameState(game);

			Menu& menu = data.GetMenu();
			if (!menu.GetSelectedItem()) return;

			if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu.GetSelectedItem() == &data.GetResumeItem()) PopGameState(game);
				else if (menu.GetSelectedItem() == &data.GetExitItem()) SwitchGameState(game, GameStateType::MainMenu);
			}

			if (event.key.code == sf::Keyboard::Up) SelectPreviousMenuItem(menu);
			else if (event.key.code == sf::Keyboard::Down) SelectNextMenuItem(menu);
		}
	}

	void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, float timeDelta) {}

	void DrawGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		data.GetBackground().setSize(viewSize);
		window.draw(data.GetBackground());

		data.GetTitleText().setOrigin(GetTextOrigin(data.GetTitleText(), { 0.5f, 0.f }));
		data.GetTitleText().setPosition(viewSize.x / 2.f, 100.f);
		window.draw(data.GetTitleText());

		DrawMenu(data.GetMenu(), window, window.getView().getCenter(), { 0.5f, 0.f });
	}
}