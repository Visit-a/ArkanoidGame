#pragma once
#include "Brick.h"
#include <vector>

namespace SnakeGame
{
    class DurableBrick : public Brick
    {
    public:
        DurableBrick(const sf::Texture& texture,
                     const sf::Vector2f& position,
                     const sf::Vector2f& size,
                     const std::vector<sf::Color>& damageColors);

        bool OnHit() override;
        void Draw(sf::RenderWindow& window) const override;

        int GetHitPoints() const { return hitPoints; }

    private:
        int hitPoints = 3;
        std::vector<sf::Color> damageColors;
        sf::Vector2f blockSize;
    };
}
