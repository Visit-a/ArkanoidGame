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
        void Destroy() { destroyed = true; }
        virtual int GetTypeId() const { return 0; } // 0 = обычный блок
        virtual int GetHitPoints() const { return destroyed ? 0 : 1; }
        virtual void RestoreState(bool isDestroyed, int hitPoints)
        {
            destroyed = isDestroyed;
        }
        sf::FloatRect GetGlobalBounds() const { return sprite.getGlobalBounds(); }

    protected:
        sf::Sprite sprite;
        bool destroyed = false;
    };
}
