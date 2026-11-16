#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
    // Общий интерфейс игровых объектов.
    class GameObject
    {
    public:
        virtual ~GameObject() = default;
        virtual void Update(float timeDelta) = 0;
        virtual void Draw(sf::RenderWindow& window) const = 0;
        virtual sf::FloatRect GetGlobalBounds() const = 0;
        virtual bool IsActive() const { return active; }

    protected:
        bool active = true;
    };
}
