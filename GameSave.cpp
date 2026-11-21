#include "GameSave.h"
#include <fstream>
#include <iomanip>

namespace SnakeGame
{
    bool SaveManager::Save(const GameMemento& s) const
    {
        std::ofstream file(fileName.c_str(), std::ios::out | std::ios::trunc);
        if (!file.is_open()) return false;

        file << "ARKANOID_SAVE\n"
             << s.score << ' ' << s.lives << '\n'
             << std::setprecision(9)
             << s.ballX << ' ' << s.ballY << ' '
             << s.velocityX << ' ' << s.velocityY << '\n'
             << s.platformX << ' ' << s.platformWidth << '\n'
             << s.bricks.size() << '\n';

        for (const BrickSnapshot& brick : s.bricks)
            file << brick.type << ' ' << brick.hitPoints << ' '
                 << (brick.destroyed ? 1 : 0) << '\n';

        return file.good();
    }

    bool SaveManager::Load(GameMemento& s) const
    {
        std::ifstream file(fileName.c_str());
        if (!file.is_open()) return false;

        std::string signature;
        if (!(file >> signature) || signature != "ARKANOID_SAVE")
            return false;

        GameMemento loaded;
        std::size_t count = 0;
        if (!(file >> loaded.score >> loaded.lives
                   >> loaded.ballX >> loaded.ballY
                   >> loaded.velocityX >> loaded.velocityY
                   >> loaded.platformX >> loaded.platformWidth
                   >> count))
            return false;

        if (count > 1000 || loaded.lives < 0 || loaded.lives > 99)
            return false;

        loaded.bricks.resize(count);
        for (BrickSnapshot& brick : loaded.bricks)
        {
            int destroyed = 0;
            if (!(file >> brick.type >> brick.hitPoints >> destroyed))
                return false;
            if (brick.type < 0 || brick.type > 2 ||
                brick.hitPoints < 0 || brick.hitPoints > 3)
                return false;
            brick.destroyed = destroyed != 0;
        }

        s = loaded;
        return true;
    }
}
