#include "Application.h"
#include "GameSettings.h"
#include <cstdlib>

namespace SnakeGame
{

	Application::Application(const std::string& gameName) :
		window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEGHT), "ArkanoidGame")
	{
		// Init random number generator
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);

		InitGame(game);
	}

	Application::~Application()
	{
		ShutdownGame(game);
	}

	void Application::Run()
	{
		sf::Clock gameClock;

		while (window.isOpen()) {
			float startTime = gameClock.getElapsedTime().asSeconds();

			HandleWindowEvents(game, window);

			if (!window.isOpen()) {
				break;
			}

			if (UpdateGame(game, TIME_PER_FRAME))
			{
				window.clear();
				DrawGame(game, window);
				window.display();
			}
			else
			{
				window.close();
			}

			float endTime = gameClock.getElapsedTime().asSeconds();
			float deltaTime = endTime - startTime;
			if (deltaTime < TIME_PER_FRAME) {
				sf::sleep(sf::seconds(TIME_PER_FRAME - deltaTime));
			}
		}
	}
}