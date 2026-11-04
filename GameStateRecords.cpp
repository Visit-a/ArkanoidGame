#include "GameStateRecords.h"
#include "Text.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include <sstream>
#include <map>

namespace SnakeGame
{
	void InitGameStateRecords(GameStateRecordsData& data, Game& game)
	{
		assert(data.GetFont().loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		data.GetTitleText().setString("RECORDS");
		data.GetTitleText().setFont(data.GetFont());
		data.GetTitleText().setFillColor(sf::Color::Red);
		data.GetTitleText().setCharacterSize(48);

		data.GetTableTexts().clear();
		data.GetTableTexts().reserve(MAX_RECORDS_TABLE_SIZE);

		
		std::multimap<int, std::string> sortedRecordsTable;
		for (const auto& item : game.recordsTable)
		{
			sortedRecordsTable.insert(std::make_pair(item.second, item.first));
		}

		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
		{
			data.GetTableTexts().emplace_back();
			sf::Text& text = data.GetTableTexts().back();

			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text.setString(sstream.str());
			text.setFont(data.GetFont());
			text.setFillColor(sf::Color::White);
			text.setCharacterSize(24);
		}

		data.GetHintText().setString("Press ESC to return back to main menu");
		data.GetHintText().setFont(data.GetFont());
		data.GetHintText().setFillColor(sf::Color::White);
		data.GetHintText().setCharacterSize(24);
	}

	void ShutdownGameStateRecords(GameStateRecordsData& data, Game& game) {}

	void HandleGameStateRecordsWindowEvent(GameStateRecordsData& data, Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			PopGameState(game);
	}

	void UpdateGameStateRecords(GameStateRecordsData& data, Game& game, float timeDelta) {}

	void DrawGameStateRecords(GameStateRecordsData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		data.GetTitleText().setOrigin(GetTextOrigin(data.GetTitleText(), { 0.5f, 0.f }));
		data.GetTitleText().setPosition(viewSize.x / 2.f, 50.f);
		window.draw(data.GetTitleText());

		std::vector<sf::Text*> textsList;
		for (auto& text : data.GetTableTexts())
			textsList.push_back(&text);

		sf::Vector2f tablePosition = { data.GetTitleText().getGlobalBounds().left, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.f, 0.f });

		data.GetHintText().setOrigin(GetTextOrigin(data.GetHintText(), { 0.5f, 1.f }));
		data.GetHintText().setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(data.GetHintText());
	}
}