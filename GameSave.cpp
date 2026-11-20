#include "GameSave.h"
#include <fstream>

namespace SnakeGame
{
    bool SaveManager::Save(const GameMemento& s) const
    {
        std::ofstream file(fileName.c_str(), std::ios::out | std::ios::trunc);
        if (!file.is_open()) return false;

        file << s.score << '\n'
             << s.bestScore << '\n'
             << s.ballX << '\n'
             << s.ballY << '\n'
             << s.velocityX << '\n'
             << s.velocityY << '\n'
             << s.platformX << '\n'
             << s.remainingBlocks << '\n';

        return file.good();
    }

    bool SaveManager::Load(GameMemento& s) const
    {
        std::ifstream file(fileName.c_str());
        if (!file.is_open()) return false;

        GameMemento loaded;
        if (!(file >> loaded.score
                   >> loaded.bestScore
                   >> loaded.ballX
                   >> loaded.ballY
                   >> loaded.velocityX
                   >> loaded.velocityY
                   >> loaded.platformX
                   >> loaded.remainingBlocks))
            return false;

        s = loaded;
        return true;
    }
}
