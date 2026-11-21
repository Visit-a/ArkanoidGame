#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace SnakeGame
{
    class Platform : public GameObject
    {
    public:
        Platform() = default;
        Platform(const sf::Texture& texture, float width, float height, sf::Vector2f position);

        void Move(float shiftX, float screenWidth);
        void Update(float) override {}
        void Draw(sf::RenderWindow& window) const override;

        const sf::Sprite& GetSprite() const { return sprite; }
        sf::Vector2f GetPosition() const { return sprite.getPosition(); }
        float GetWidth() const { return sprite.getGlobalBounds().width; }
        void SetWidth(float width);
        sf::FloatRect GetGlobalBounds() const override { return sprite.getGlobalBounds(); }

    private:
        sf::Sprite sprite;
    };
}
