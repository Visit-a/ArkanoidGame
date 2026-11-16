#pragma once
#include <string>

namespace SnakeGame
{
    const std::string RESOURCES_PATH = "Resources/";
    const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
    const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
    const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

    const unsigned int SCREEN_WIDTH = 800;
    const unsigned int SCREEN_HEGHT = 600;
    const float TIME_PER_FRAME = 1.f / 60.f;

    const float PLATFORM_WIDTH = 100.f;
    const float PLATFORM_HEIGHT = 20.f;
    const float PLATFORM_SPEED = 500.f;

    const float BALL_RADIUS = 10.f;
    const float BALL_SPEED = 400.f;

    const float BLOCK_WIDTH = 68.f;
    const float BLOCK_HEIGHT = 22.f;
    const int BLOCK_COLUMNS = 10;
    const int BLOCK_ROWS = 4;

    const int MAX_RECORDS_TABLE_SIZE = 5;
    extern const char* PLAYER_NAME;
}
