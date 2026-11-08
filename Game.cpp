#include "Game.h"
#include <assert.h>
#include <algorithm>

#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"

namespace SnakeGame
{
	void InitGame(Game& game)
	{
		game.recordsTable =
		{
			{"John", 40},
			{"Jane", 27},
			{"Alice", 20},
			{"Bob", 16},
			{"Clementine", 16},
		};

		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
		SwitchGameState(game, GameStateType::MainMenu);
	}

	void HandleWindowEvents(Game& game, sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (!game.gameStateStack.empty())
			{
				HandleWindowEventGameState(game, game.gameStateStack.back(), event);
			}
		}
	}

	bool UpdateGame(Game& game, float timeDelta)
	{
		if (game.gameStateChangeType == GameStateChangeType::Switch)
		{
			while (!game.gameStateStack.empty())
			{
				ShutdownGameState(game, game.gameStateStack.back());
				game.gameStateStack.pop_back();
			}
		}
		else if (game.gameStateChangeType == GameStateChangeType::Pop)
		{
			if (!game.gameStateStack.empty())
			{
				ShutdownGameState(game, game.gameStateStack.back());
				game.gameStateStack.pop_back();
			}
		}

		if (game.pendingGameStateType != GameStateType::None)
		{
			GameState newState;
			InitGameState(game, newState);
			game.gameStateStack.push_back(std::move(newState));
		}

		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;

		if (!game.gameStateStack.empty())
		{
			UpdateGameState(game, game.gameStateStack.back(), timeDelta);
			return true;
		}

		return false;
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		if (!game.gameStateStack.empty())
		{
			std::vector<GameState*> visibleGameStates;
			for (auto it = game.gameStateStack.rbegin(); it != game.gameStateStack.rend(); ++it)
			{
				visibleGameStates.push_back(&(*it));
				if (it->IsExclusivelyVisible())
				{
					break;
				}
			}

			for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
			{
				DrawGameState(game, **it, window);
			}
		}
	}

	void ShutdownGame(Game& game)
	{
		while (!game.gameStateStack.empty())
		{
			ShutdownGameState(game, game.gameStateStack.back());
			game.gameStateStack.pop_back();
		}

		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
	}

	void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible)
	{
		game.pendingGameStateType = stateType;
		game.pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		game.gameStateChangeType = GameStateChangeType::Push;
	}

	void PopGameState(Game& game)
	{
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
		game.gameStateChangeType = GameStateChangeType::Pop;
	}

	void SwitchGameState(Game& game, GameStateType newState)
	{
		game.pendingGameStateType = newState;
		game.pendingGameStateIsExclusivelyVisible = false;
		game.gameStateChangeType = GameStateChangeType::Switch;
	}

	void InitGameState(Game& game, GameState& state)
	{
		switch (game.pendingGameStateType)
		{
		case GameStateType::MainMenu:
			state.Init<GameStateMainMenuData>(GameStateType::MainMenu, game.pendingGameStateIsExclusivelyVisible, game, InitGameStateMainMenu);
			break;
		case GameStateType::Playing:
			state.Init<GameStatePlayingData>(GameStateType::Playing, game.pendingGameStateIsExclusivelyVisible, game, InitGameStatePlaying);
			break;
		case GameStateType::GameOver:
			state.Init<GameStateGameOverData>(GameStateType::GameOver, game.pendingGameStateIsExclusivelyVisible, game, InitGameStateGameOver);
			break;
		case GameStateType::ExitDialog:
			state.Init<GameStatePauseMenuData>(GameStateType::ExitDialog, game.pendingGameStateIsExclusivelyVisible, game, InitGameStatePauseMenu);
			break;
		case GameStateType::Records:
			state.Init<GameStateRecordsData>(GameStateType::Records, game.pendingGameStateIsExclusivelyVisible, game, InitGameStateRecords);
			break;
		default:
			assert(false);
			break;
		}
	}

	void ShutdownGameState(Game& game, GameState& state)
	{
		switch (state.GetType())
		{
		case GameStateType::MainMenu: ShutdownGameStateMainMenu(*state.GetDataAs<GameStateMainMenuData>(), game); break;
		case GameStateType::Playing: ShutdownGameStatePlaying(*state.GetDataAs<GameStatePlayingData>(), game); break;
		case GameStateType::GameOver: ShutdownGameStateGameOver(*state.GetDataAs<GameStateGameOverData>(), game); break;
		case GameStateType::ExitDialog: ShutdownGameStatePauseMenu(*state.GetDataAs<GameStatePauseMenuData>(), game); break;
		case GameStateType::Records: ShutdownGameStateRecords(*state.GetDataAs<GameStateRecordsData>(), game); break;
		default: assert(false); break;
		}
	}

	void HandleWindowEventGameState(Game& game, GameState& state, sf::Event& event)
	{
		switch (state.GetType())
		{
		case GameStateType::MainMenu: HandleGameStateMainMenuWindowEvent(*state.GetDataAs<GameStateMainMenuData>(), game, event); break;
		case GameStateType::Playing: HandleGameStatePlayingWindowEvent(*state.GetDataAs<GameStatePlayingData>(), game, event); break;
		case GameStateType::GameOver: HandleGameStateGameOverWindowEvent(*state.GetDataAs<GameStateGameOverData>(), game, event); break;
		case GameStateType::ExitDialog: HandleGameStatePauseMenuWindowEvent(*state.GetDataAs<GameStatePauseMenuData>(), game, event); break;
		case GameStateType::Records: HandleGameStateRecordsWindowEvent(*state.GetDataAs<GameStateRecordsData>(), game, event); break;
		default: assert(false); break;
		}
	}

	void UpdateGameState(Game& game, GameState& state, float timeDelta)
	{
		switch (state.GetType())
		{
		case GameStateType::MainMenu: UpdateGameStateMainMenu(*state.GetDataAs<GameStateMainMenuData>(), game, timeDelta); break;
		case GameStateType::Playing: UpdateGameStatePlaying(*state.GetDataAs<GameStatePlayingData>(), game, timeDelta); break;
		case GameStateType::GameOver: UpdateGameStateGameOver(*state.GetDataAs<GameStateGameOverData>(), game, timeDelta); break;
		case GameStateType::ExitDialog: UpdateGameStatePauseMenu(*state.GetDataAs<GameStatePauseMenuData>(), game, timeDelta); break;
		case GameStateType::Records: UpdateGameStateRecords(*state.GetDataAs<GameStateRecordsData>(), game, timeDelta); break;
		default: assert(false); break;
		}
	}

	void DrawGameState(Game& game, GameState& state, sf::RenderWindow& window)
	{
		switch (state.GetType())
		{
		case GameStateType::MainMenu: DrawGameStateMainMenu(*state.GetDataAs<GameStateMainMenuData>(), game, window); break;
		case GameStateType::Playing: DrawGameStatePlaying(*state.GetDataAs<GameStatePlayingData>(), game, window); break;
		case GameStateType::GameOver: DrawGameStateGameOver(*state.GetDataAs<GameStateGameOverData>(), game, window); break;
		case GameStateType::ExitDialog: DrawGameStatePauseMenu(*state.GetDataAs<GameStatePauseMenuData>(), game, window); break;
		case GameStateType::Records: DrawGameStateRecords(*state.GetDataAs<GameStateRecordsData>(), game, window); break;
		default: assert(false); break;
		}
	}
}