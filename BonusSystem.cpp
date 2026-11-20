#include "BonusSystem.h"
#include <cstdlib>
#include <ctime>

namespace SnakeGame
{
    namespace
    {
        const IBonusEffect& EffectByType(int type)
        {
            static FireballEffect fireball;
            static FragileBlocksEffect fragile;
            static PlatformBoostEffect platform;
            if (type == 0) return fireball;
            if (type == 1) return fragile;
            return platform;
        }
    }

    BonusDrop::BonusDrop(const sf::Vector2f& position, int bonusType)
        : type(bonusType), fallSpeed(150.f)
    {
        shape.setRadius(9.f);
        shape.setOrigin(9.f, 9.f);
        shape.setPosition(position);
        if (type == 0) shape.setFillColor(sf::Color(255, 90, 40));
        else if (type == 1) shape.setFillColor(sf::Color(80, 220, 120));
        else shape.setFillColor(sf::Color(80, 170, 255));
    }

    void BonusDrop::Update(float dt)
    {
        shape.move(0.f, fallSpeed * dt);
    }

    void BonusDrop::Draw(sf::RenderWindow& window) const { window.draw(shape); }

    bool BonusDrop::IsCaught(const sf::FloatRect& bounds) const
    {
        return shape.getGlobalBounds().intersects(bounds);
    }

    bool BonusDrop::IsOutOfScreen(float screenHeight) const
    {
        return shape.getPosition().y - shape.getRadius() > screenHeight;
    }

    const IBonusEffect& BonusDrop::GetEffect() const { return EffectByType(type); }

    BonusManager::BonusManager() : activeTime(0.f), activeDuration(7.f)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void BonusManager::TrySpawn(const sf::Vector2f& position)
    {
        if (std::rand() % 100 < 10)
        {
            const int type = std::rand() % 3;
            drops.push_back(BonusDrop(position, type));
        }
    }

    void BonusManager::Activate(int type)
    {
        context = BonusContext();
        EffectByType(type).Apply(context);
        activeTime = activeDuration;
    }

    void BonusManager::Update(
        float dt, const sf::FloatRect& platformBounds, float screenHeight)
    {
        for (std::size_t i = 0; i < drops.size();)
        {
            drops[i].Update(dt);
            if (drops[i].IsCaught(platformBounds))
            {
                // Определяем тип бонуса через цвет/положение не требуется:
                // для активации используем эффект объекта напрямую.
                Activate(drops[i].GetType());
                drops.erase(drops.begin() + i);
            }
            else if (drops[i].IsOutOfScreen(screenHeight))
            {
                drops.erase(drops.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        if (activeTime > 0.f)
        {
            activeTime -= dt;
            if (activeTime <= 0.f)
            {
                activeTime = 0.f;
                context = BonusContext();
            }
        }
    }

    void BonusManager::Draw(sf::RenderWindow& window) const
    {
        for (const BonusDrop& drop : drops)
            drop.Draw(window);
    }
}
