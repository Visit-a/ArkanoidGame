#include "DurableBrick.h"
#include <algorithm>

namespace SnakeGame
{
    DurableBrick::DurableBrick(const sf::Texture& texture,
                               const sf::Vector2f& position,
                               const sf::Vector2f& size,
                               const std::vector<sf::Color>& colors)
        : Brick(texture, position, size),
          damageColors(colors),
          blockSize(size)
    {
        // Крепкий блок выделяется оттенком текстуры, без внешней обводки.
        sprite.setColor(sf::Color(210, 125, 255));
    }

    bool DurableBrick::OnHit()
    {
        if (destroyed)
            return true;

        --hitPoints;

        if (hitPoints <= 0)
        {
            destroyed = true;
        }
        else if (!damageColors.empty())
        {
            const std::size_t colorIndex = std::min<std::size_t>(
                damageColors.size() - 1,
                static_cast<std::size_t>(3 - hitPoints));

            sprite.setColor(damageColors[colorIndex]);
        }

        return true;
    }

    void DurableBrick::Draw(sf::RenderWindow& window) const
    {
        if (destroyed)
            return;

        // Рисуем только текстуру без рамки. Индикаторы показывают оставшуюся прочность.
        window.draw(sprite);

        const float pipWidth = 6.f;
        const float pipHeight = 4.f;
        const float pipGap = 2.f;
        const float totalPipsWidth = 3.f * pipWidth + 2.f * pipGap;
        const float startX = sprite.getPosition().x - totalPipsWidth / 2.f;
        const float pipY = sprite.getPosition().y - blockSize.y / 2.f + 4.f;

        for (int i = 0; i < 3; ++i)
        {
            sf::RectangleShape pip(sf::Vector2f(pipWidth, pipHeight));
            pip.setPosition(startX + i * (pipWidth + pipGap), pipY);
            pip.setFillColor(i < hitPoints
                ? sf::Color::White
                : sf::Color(70, 70, 70, 180));
            window.draw(pip);
        }
    }
}
