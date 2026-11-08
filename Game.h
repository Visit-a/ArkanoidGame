#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace SnakeGame
{
	enum class GameOptions : std::uint8_t
	{
		InfiniteApples = 1 << 0,
		WithAcceleration = 1 << 1,
		Default = InfiniteApples | WithAcceleration,
		Empty = 0
	};

	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		ExitDialog,
		Records,
	};

	enum class GameStateChangeType
	{
		None,
		Push,
		Pop,
		Switch
	};

	class Game;

	class GameState
	{
	public:
		GameState() = default;

		GameState(const GameState&) = delete;
		GameState& operator=(const GameState&) = delete;
		GameState(GameState&&) noexcept = default;
		GameState& operator=(GameState&&) noexcept = default;


		template<typename T>
		void Init(GameStateType type, bool isExclusivelyVisible, Game& game, void(*initFunc)(T&, Game&))
		{
			this->type = type;
			std::unique_ptr<T> tempPtr = std::make_unique<T>();
			initFunc(*tempPtr, game);
			this->data = std::unique_ptr<void, void(*)(void*)>(tempPtr.release(), [](void* p) {
				delete static_cast<T*>(p);
				});
			this->isExclusivelyVisible = isExclusivelyVisible;
		}

		GameStateType GetType() const { return type; }
		bool IsExclusivelyVisible() const { return isExclusivelyVisible; }
		void* GetData() const { return data.get(); }

		template<typename T>
		T* GetDataAs() const { return static_cast<T*>(data.get()); }

	private:
		GameStateType type = GameStateType::None;
		std::unique_ptr<void, void(*)(void*)> data = { nullptr, [](void*) {} };
		bool isExclusivelyVisible = false;
	};

	class Game
	{
	public:
		std::vector<GameState> gameStateStack;
		GameStateChangeType gameStateChangeType = GameStateChangeType::None;
		GameStateType pendingGameStateType = GameStateType::None;
		bool pendingGameStateIsExclusivelyVisible = false;

		GameOptions options = GameOptions::Default;
		std::unordered_map<std::string, int> recordsTable;
	};

	void InitGame(Game& game);
	void HandleWindowEvents(Game& game, sf::RenderWindow& window);
	bool UpdateGame(Game& game, float timeDelta);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void ShutdownGame(Game& game);

	void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible);
	void PopGameState(Game& game);
	void SwitchGameState(Game& game, GameStateType newState);

	void InitGameState(Game& game, GameState& state);
	void ShutdownGameState(Game& game, GameState& state);
	void HandleWindowEventGameState(Game& game, GameState& state, sf::Event& event);
	void UpdateGameState(Game& game, GameState& state, float timeDelta);
	void DrawGameState(Game& game, GameState& state, sf::RenderWindow& window);
}