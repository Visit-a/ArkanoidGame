#include "GameStateMainMenu.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>

namespace SnakeGame
{
	void InitGameStateMainMenu(GameStateMainMenuData& data, Game& game)
	{
		assert(data.GetFont().loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		Menu& menu = data.GetMenu();
		MenuItem& root = menu.GetRootItem();

		root.GetHintText().setString("Arkanoid");
		root.GetHintText().setFont(data.GetFont());
		root.GetHintText().setCharacterSize(48);
		root.GetHintText().setFillColor(sf::Color::Red);
		root.SetChildrenOrientation(Orientation::Vertical);
		root.SetChildrenAlignment(Alignment::Middle);
		root.SetChildrenSpacing(10.f);
		root.AddChild(&data.GetStartGameItem());
		root.AddChild(&data.GetOptionsItem());
		root.AddChild(&data.GetRecordsItem());
		root.AddChild(&data.GetExitGameItem());

		data.GetStartGameItem().GetText().setString("Start Game");
		data.GetStartGameItem().GetText().setFont(data.GetFont());
		data.GetStartGameItem().GetText().setCharacterSize(24);

		data.GetOptionsItem().GetText().setString("Options");
		data.GetOptionsItem().GetText().setFont(data.GetFont());
		data.GetOptionsItem().GetText().setCharacterSize(24);
		data.GetOptionsItem().GetHintText().setString("Options");
		data.GetOptionsItem().GetHintText().setFont(data.GetFont());
		data.GetOptionsItem().GetHintText().setCharacterSize(48);
		data.GetOptionsItem().GetHintText().setFillColor(sf::Color::Red);
		data.GetOptionsItem().SetChildrenOrientation(Orientation::Vertical);
		data.GetOptionsItem().SetChildrenAlignment(Alignment::Middle);
		data.GetOptionsItem().SetChildrenSpacing(10.f);
		data.GetOptionsItem().AddChild(&data.GetOptionsInfiniteApplesItem());
		data.GetOptionsItem().AddChild(&data.GetOptionsWithAccelerationItem());

		data.GetOptionsInfiniteApplesItem().GetText().setString("Infinite Apples: On/Off");
		data.GetOptionsInfiniteApplesItem().GetText().setFont(data.GetFont());
		data.GetOptionsInfiniteApplesItem().GetText().setCharacterSize(24);

		data.GetOptionsWithAccelerationItem().GetText().setString("With Acceleration: On/Off");
		data.GetOptionsWithAccelerationItem().GetText().setFont(data.GetFont());
		data.GetOptionsWithAccelerationItem().GetText().setCharacterSize(24);

		data.GetRecordsItem().GetText().setString("Records");
		data.GetRecordsItem().GetText().setFont(data.GetFont());
		data.GetRecordsItem().GetText().setCharacterSize(24);

		data.GetExitGameItem().GetText().setString("Exit Game");
		data.GetExitGameItem().GetText().setFont(data.GetFont());
		data.GetExitGameItem().GetText().setCharacterSize(24);
		data.GetExitGameItem().GetHintText().setString("Are you sure?");
		data.GetExitGameItem().GetHintText().setFont(data.GetFont());
		data.GetExitGameItem().GetHintText().setCharacterSize(48);
		data.GetExitGameItem().GetHintText().setFillColor(sf::Color::Red);
		data.GetExitGameItem().SetChildrenOrientation(Orientation::Horizontal);
		data.GetExitGameItem().SetChildrenAlignment(Alignment::Middle);
		data.GetExitGameItem().SetChildrenSpacing(10.f);
		data.GetExitGameItem().AddChild(&data.GetYesItem());
		data.GetExitGameItem().AddChild(&data.GetNoItem());

		data.GetYesItem().GetText().setString("Yes");
		data.GetYesItem().GetText().setFont(data.GetFont());
		data.GetYesItem().GetText().setCharacterSize(24);

		data.GetNoItem().GetText().setString("No");
		data.GetNoItem().GetText().setFont(data.GetFont());
		data.GetNoItem().GetText().setCharacterSize(24);

		InitMenuItem(menu.GetRootItem());
		SelectMenuItem(menu, &data.GetStartGameItem());
	}

	void ShutdownGameStateMainMenu(GameStateMainMenuData& data, Game& game) {}

	void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, Game& game, const sf::Event& event)
	{
		Menu& menu = data.GetMenu();
		if (!menu.GetSelectedItem()) return;

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				CollapseSelectedItem(menu);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (menu.GetSelectedItem() == &data.GetStartGameItem()) SwitchGameState(game, GameStateType::Playing);
				else if (menu.GetSelectedItem() == &data.GetOptionsItem()) ExpandSelectedItem(menu);
				else if (menu.GetSelectedItem() == &data.GetOptionsInfiniteApplesItem()) game.options = (GameOptions)((std::uint8_t)game.options ^ (std::uint8_t)GameOptions::InfiniteApples);
				else if (menu.GetSelectedItem() == &data.GetOptionsWithAccelerationItem()) game.options = (GameOptions)((std::uint8_t)game.options ^ (std::uint8_t)GameOptions::WithAcceleration);
				else if (menu.GetSelectedItem() == &data.GetRecordsItem()) PushGameState(game, GameStateType::Records, true);
				else if (menu.GetSelectedItem() == &data.GetExitGameItem()) ExpandSelectedItem(menu);
				else if (menu.GetSelectedItem() == &data.GetYesItem()) SwitchGameState(game, GameStateType::None);
				else if (menu.GetSelectedItem() == &data.GetNoItem()) CollapseSelectedItem(menu);
				else ExpandSelectedItem(menu);
			}

			Orientation orientation = menu.GetSelectedItem()->GetParent()->GetChildrenOrientation();
			if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up) ||
				(orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left))
				SelectPreviousMenuItem(menu);
			else if ((orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down) ||
				(orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right))
				SelectNextMenuItem(menu);
		}
	}

	void UpdateGameStateMainMenu(GameStateMainMenuData& data, Game& game, float timeDelta)
	{
		bool isInfiniteApples = ((std::uint8_t)game.options & (std::uint8_t)GameOptions::InfiniteApples) != 0;
		data.GetOptionsInfiniteApplesItem().GetText().setString("Infinite Apples: " + std::string(isInfiniteApples ? "On" : "Off"));

		bool isWithAcceleration = ((std::uint8_t)game.options & (std::uint8_t)GameOptions::WithAcceleration) != 0;
		data.GetOptionsWithAccelerationItem().GetText().setString("With Acceleration: " + std::string(isWithAcceleration ? "On" : "Off"));
	}

	void DrawGameStateMainMenu(GameStateMainMenuData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();
		Menu& menu = data.GetMenu();

		sf::Text* hintText = &GetCurrentMenuContext(menu)->GetHintText();
		hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 150.f);
		window.draw(*hintText);

		DrawMenu(menu, window, viewSize / 2.f, { 0.5f, 0.f });
	}
}