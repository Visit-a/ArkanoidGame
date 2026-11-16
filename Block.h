#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace SnakeGame
{
    class Block : public GameObject
    {
    public:
        Block() = default;
        Block(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f size);

        void Update(float) override {}
        void Draw(sf::RenderWindow& window) const override;
        sf::FloatRect GetGlobalBounds() const override;
        void Destroy() { active = false; }

    private:
        sf::Sprite sprite;
    };
}
