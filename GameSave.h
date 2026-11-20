#pragma once
#include <string>

namespace SnakeGame
{
    struct GameMemento
    {
        int score = 0;
        int bestScore = 0;
        float ballX = 0.f;
        float ballY = 0.f;
        float velocityX = 0.f;
        float velocityY = 0.f;
        float platformX = 0.f;
        int remainingBlocks = 0;
    };

    class SaveManager
    {
    public:
        explicit SaveManager(const std::string& fileName = "savegame.dat")
            : fileName(fileName) {}

        bool Save(const GameMemento& state) const;
        bool Load(GameMemento& state) const;

    private:
        std::string fileName;
    };
}
