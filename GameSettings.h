#pragma once
#include <string>

namespace SnakeGame
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";
	const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
	const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
	const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

	// Screen
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEGHT = 600;
	const float TIME_PER_FRAME = 1.f / 60.f; // 60 fps

	// Platform settings
	const float PLATFORM_WIDTH = 100.f;
	const float PLATFORM_HEIGHT = 20.f;
	const float PLATFORM_SPEED = 500.f; // pixels per second

	// Ball settings
	const float BALL_RADIUS = 10.f;
	const float BALL_SPEED = 400.f; // pixels per second

	// Records
	const int MAX_RECORDS_TABLE_SIZE = 5;
	extern const char* PLAYER_NAME; // Defined in GameStateGameOver.cpp
}