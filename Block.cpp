#include "Block.h"

namespace SnakeGame
{
    Block::Block(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f size)
    {
        sprite.setTexture(texture);

        const sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setScale(size.x / bounds.width, size.y / bounds.height);
        sprite.setPosition(position);
    }

    void Block::Draw(sf::RenderWindow& window) const
    {
        if (active)
            window.draw(sprite);
    }

    sf::FloatRect Block::GetGlobalBounds() const
    {
        return sprite.getGlobalBounds();
    }
}
