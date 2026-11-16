#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
    class Game;
    class GameStateWinData
    {
    public:
        sf::Font& GetFont() { return font; }
        sf::Text& GetTitle() { return title; }
        sf::Text& GetQuestion() { return question; }
        sf::Text& GetHint() { return hint; }
        friend void InitGameStateWin(GameStateWinData&, Game&);
        friend void DrawGameStateWin(GameStateWinData&, Game&, sf::RenderWindow&);
    private:
        sf::Font font;
        sf::Text title;
        sf::Text question;
        sf::Text hint;
    };

    void InitGameStateWin(GameStateWinData& data, Game& game);
    void ShutdownGameStateWin(GameStateWinData& data, Game& game);
    void HandleGameStateWinWindowEvent(GameStateWinData& data, Game& game, const sf::Event& event);
    void UpdateGameStateWin(GameStateWinData& data, Game& game, float timeDelta);
    void DrawGameStateWin(GameStateWinData& data, Game& game, sf::RenderWindow& window);
}
