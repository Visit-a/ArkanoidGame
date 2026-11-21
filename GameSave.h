#pragma once
#include <string>
#include <vector>

namespace SnakeGame
{
    struct BrickSnapshot
    {
        int type = 0;       // 0 — обычный, 1 — крепкий, 2 — стеклянный
        int hitPoints = 1;
        bool destroyed = false;
    };

    struct GameMemento
    {
        int score = 0;
        int lives = 3;
        float ballX = 0.f;
        float ballY = 0.f;
        float velocityX = 0.f;
        float velocityY = 0.f;
        float platformX = 0.f;
        float platformWidth = 100.f;
        std::vector<BrickSnapshot> bricks;
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
