#include "GlassBrick.h"

namespace SnakeGame
{
    GlassBrick::GlassBrick(const sf::Texture& texture,
                           const sf::Vector2f& position,
                           const sf::Vector2f& size)
        : Brick(texture, position, size)
    {
        // Та же Block.png, только прозрачнее. Форма и текстура не меняются.
        sprite.setColor(sf::Color(255, 255, 255, 100));
    }

    bool GlassBrick::OnHit()
    {
        destroyed = true;
        // Стеклянный блок исчезает, не меняя направление мяча.
        return false;
    }

    void GlassBrick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(sprite);
    }
}
