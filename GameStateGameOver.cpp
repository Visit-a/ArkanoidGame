#include "GameStateGameOver.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

namespace SnakeGame
{
	const char* PLAYER_NAME = "Player";

	void InitGameStateGameOver(GameStateGameOverData& data, Game& game)
	{
		assert(data.GetFont().loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		data.GetTimeSinceGameOver() = 0.f;

		sf::Color backgroundColor = sf::Color::Black;
		backgroundColor.a = 200;
		data.GetBackground().setFillColor(backgroundColor);

		data.GetGameOverText().setFont(data.GetFont());
		data.GetGameOverText().setCharacterSize(48);
		data.GetGameOverText().setStyle(sf::Text::Bold);
		data.GetGameOverText().setFillColor(sf::Color::Red);
		data.GetGameOverText().setString("GAME OVER");

		data.GetRecordsTableTexts().clear();
		data.GetRecordsTableTexts().reserve(MAX_RECORDS_TABLE_SIZE);

		std::multimap<int, std::string> sortedRecordsTable;
		int playerScore = game.recordsTable[PLAYER_NAME];
		for (const auto& item : game.recordsTable)
		{
			sortedRecordsTable.insert(std::make_pair(item.second, item.first));
		}

		bool isPlayerInTable = false;
		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
		{
			data.GetRecordsTableTexts().emplace_back();
			sf::Text& text = data.GetRecordsTableTexts().back();

			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(data.GetFont());
			text.setCharacterSize(24);

			if (it->second == PLAYER_NAME)
			{
				text.setFillColor(sf::Color::Green);
				isPlayerInTable = true;
			}
			else
			{
				text.setFillColor(sf::Color::White);
			}
		}

		if (!isPlayerInTable && !data.GetRecordsTableTexts().empty())
		{
			sf::Text& text = data.GetRecordsTableTexts().back();
			std::stringstream sstream;
			sstream << MAX_RECORDS_TABLE_SIZE << ". " << PLAYER_NAME << ": " << playerScore;
			text.setString(sstream.str());
			text.setFillColor(sf::Color::Green);
		}

		data.GetHintText().setFont(data.GetFont());
		data.GetHintText().setCharacterSize(24);
		data.GetHintText().setFillColor(sf::Color::White);
		data.GetHintText().setString("Press Space to restart\nEsc to exit to main menu");
	}

	void ShutdownGameStateGameOver(GameStateGameOverData& data, Game& game) {}

	void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space) SwitchGameState(game, GameStateType::Playing);
			else if (event.key.code == sf::Keyboard::Escape) SwitchGameState(game, GameStateType::MainMenu);
		}
	}

	void UpdateGameStateGameOver(GameStateGameOverData& data, Game& game, float timeDelta)
	{
		data.GetTimeSinceGameOver() += timeDelta;
		sf::Color color = (int)data.GetTimeSinceGameOver() % 2 ? sf::Color::Red : sf::Color::Yellow;
		data.GetGameOverText().setFillColor(color);
	}

	void DrawGameStateGameOver(GameStateGameOverData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		data.GetBackground().setOrigin(0.f, 0.f);
		data.GetBackground().setSize(viewSize);
		window.draw(data.GetBackground());

		data.GetGameOverText().setOrigin(GetTextOrigin(data.GetGameOverText(), { 0.5f, 1.f }));
		data.GetGameOverText().setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 50.f);
		window.draw(data.GetGameOverText());

		std::vector<sf::Text*> textsList;
		for (auto& text : data.GetRecordsTableTexts())
			textsList.push_back(&text);

		sf::Vector2f tablePosition = { viewSize.x / 2.f, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.5f, 0.f });

		data.GetHintText().setOrigin(GetTextOrigin(data.GetHintText(), { 0.5f, 1.f }));
		data.GetHintText().setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(data.GetHintText());
	}
}