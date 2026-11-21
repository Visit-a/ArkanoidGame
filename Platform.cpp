#include "Platform.h"
#include "Sprite.h"
#include "GameSettings.h"
#include <algorithm>

namespace SnakeGame
{
    Platform::Platform(const sf::Texture& texture, float width, float height, sf::Vector2f position)
    {
        InitSprite(sprite, width, height, texture);
        sprite.setPosition(position);
    }

    void Platform::Move(float shiftX, float screenWidth)
    {
        sf::Vector2f pos = sprite.getPosition();
        pos.x += shiftX;

        const float halfWidth = sprite.getGlobalBounds().width / 2.f;
        if (pos.x - halfWidth < 0.f) pos.x = halfWidth;
        if (pos.x + halfWidth > screenWidth) pos.x = screenWidth - halfWidth;

        sprite.setPosition(pos);
    }

    void Platform::SetWidth(float width)
    {
        const sf::FloatRect local = sprite.getLocalBounds();
        if (local.width <= 0.f) return;
        const sf::Vector2f oldScale = sprite.getScale();
        sprite.setScale(width / local.width, oldScale.y);
        const float halfWidth = sprite.getGlobalBounds().width / 2.f;
        sf::Vector2f pos = sprite.getPosition();
        pos.x = std::max(halfWidth, std::min(pos.x, SCREEN_WIDTH - halfWidth));
        sprite.setPosition(pos);
    }

    void Platform::Draw(sf::RenderWindow& window) const
    {
        window.draw(sprite);
    }
}
