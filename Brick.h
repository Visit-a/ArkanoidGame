#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
    class Brick
    {
    public:
        Brick(const sf::Texture& texture,
              const sf::Vector2f& position,
              const sf::Vector2f& size);
        virtual ~Brick() = default;

        // true — мяч отскакивает; false — не отскакивает.
        virtual bool OnHit();
        virtual void Draw(sf::RenderWindow& window) const;
        virtual void Update(float timeDelta);

        bool IsDestroyed() const { return destroyed; }
        sf::FloatRect GetGlobalBounds() const { return sprite.getGlobalBounds(); }

    protected:
        sf::Sprite sprite;
        bool destroyed = false;
    };
}
