#pragma once
#include "Brick.h"

namespace SnakeGame
{
    class GlassBrick : public Brick
    {
    public:
        GlassBrick(const sf::Texture& texture,
                   const sf::Vector2f& position,
                   const sf::Vector2f& size);

        bool OnHit() override;
        void Draw(sf::RenderWindow& window) const override;
    };
}
