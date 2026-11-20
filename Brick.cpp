#include "Brick.h"

namespace SnakeGame
{
    Brick::Brick(const sf::Texture& texture,
                 const sf::Vector2f& position,
                 const sf::Vector2f& size)
    {
        sprite.setTexture(texture);

        const sf::FloatRect bounds = sprite.getLocalBounds();
        if (bounds.width > 0.f && bounds.height > 0.f)
        {
            sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            sprite.setScale(size.x / bounds.width, size.y / bounds.height);
        }

        sprite.setPosition(position);

        // Обычный блок сохраняет оригинальные цвета Block.png.
        sprite.setColor(sf::Color::White);
    }

    bool Brick::OnHit()
    {
        destroyed = true;
        return true;
    }

    void Brick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(sprite);
    }

    void Brick::Update(float) {}
}
