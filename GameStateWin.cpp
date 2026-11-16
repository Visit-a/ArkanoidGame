#include "GameStateWin.h"
#include "Game.h"
#include "GameSettings.h"
#include "Text.h"
#include <cassert>

namespace SnakeGame
{
    void InitGameStateWin(GameStateWinData& data, Game& game)
    {
        assert(data.GetFont().loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

        data.GetTitle().setFont(data.GetFont());
        data.GetTitle().setString("YOU WIN!");
        data.GetTitle().setCharacterSize(52);
        data.GetTitle().setStyle(sf::Text::Bold);
        data.GetTitle().setFillColor(sf::Color(100, 235, 140));

        data.GetQuestion().setFont(data.GetFont());
        data.GetQuestion().setString("Congratulations! All blocks are destroyed.");
        data.GetQuestion().setCharacterSize(22);
        data.GetQuestion().setFillColor(sf::Color::White);

        data.GetHint().setFont(data.GetFont());
        data.GetHint().setString("Play again?  Y - Yes     N - No");
        data.GetHint().setCharacterSize(24);
        data.GetHint().setFillColor(sf::Color::Yellow);
    }

    void ShutdownGameStateWin(GameStateWinData& data, Game& game) {}

    void HandleGameStateWinWindowEvent(GameStateWinData& data, Game& game, const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed) return;

        if (event.key.code == sf::Keyboard::Y || event.key.code == sf::Keyboard::Enter)
            SwitchGameState(game, GameStateType::Playing);
        else if (event.key.code == sf::Keyboard::N || event.key.code == sf::Keyboard::Escape)
            SwitchGameState(game, GameStateType::MainMenu);
    }

    void UpdateGameStateWin(GameStateWinData& data, Game& game, float timeDelta) {}

    void DrawGameStateWin(GameStateWinData& data, Game& game, sf::RenderWindow& window)
    {
        const sf::Vector2f size = (sf::Vector2f)window.getView().getSize();
        sf::RectangleShape overlay(size);
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        data.GetTitle().setOrigin(GetTextOrigin(data.GetTitle(), { 0.5f, 0.5f }));
        data.GetTitle().setPosition(size.x / 2.f, size.y / 2.f - 80.f);
        window.draw(data.GetTitle());

        data.GetQuestion().setOrigin(GetTextOrigin(data.GetQuestion(), { 0.5f, 0.5f }));
        data.GetQuestion().setPosition(size.x / 2.f, size.y / 2.f);
        window.draw(data.GetQuestion());

        data.GetHint().setOrigin(GetTextOrigin(data.GetHint(), { 0.5f, 0.5f }));
        data.GetHint().setPosition(size.x / 2.f, size.y / 2.f + 65.f);
        window.draw(data.GetHint());
    }
}
