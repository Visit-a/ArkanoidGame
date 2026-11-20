#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace SnakeGame
{
    struct BonusContext
    {
        bool fireball = false;
        bool fragileBlocks = false;
        float ballSpeedMultiplier = 1.f;
        float platformWidthMultiplier = 1.f;
        float platformSpeedMultiplier = 1.f;
    };

    class IBonusEffect
    {
    public:
        virtual ~IBonusEffect() {}
        virtual void Apply(BonusContext& context) const = 0;
        virtual const char* GetName() const = 0;
    };

    class FireballEffect : public IBonusEffect
    {
    public:
        void Apply(BonusContext& c) const override
        {
            c.fireball = true;
            c.ballSpeedMultiplier = 1.4f;
        }
        const char* GetName() const override { return "Огненный мяч"; }
    };

    class FragileBlocksEffect : public IBonusEffect
    {
    public:
        void Apply(BonusContext& c) const override { c.fragileBlocks = true; }
        const char* GetName() const override { return "Хрупкие блоки"; }
    };

    class PlatformBoostEffect : public IBonusEffect
    {
    public:
        void Apply(BonusContext& c) const override
        {
            c.platformWidthMultiplier = 1.5f;
            c.platformSpeedMultiplier = 1.25f;
        }
        const char* GetName() const override { return "Усиленная платформа"; }
    };

    class BonusDrop
    {
    public:
        BonusDrop(const sf::Vector2f& position, int type);
        void Update(float dt);
        void Draw(sf::RenderWindow& window) const;
        bool IsCaught(const sf::FloatRect& platformBounds) const;
        bool IsOutOfScreen(float screenHeight) const;
        const IBonusEffect& GetEffect() const;
        int GetType() const { return type; }

    private:
        sf::CircleShape shape;
        int type;
        float fallSpeed;
    };

    class BonusManager
    {
    public:
        BonusManager();

        // Вызывать при разрушении блока: бонус появляется с вероятностью 10%.
        void TrySpawn(const sf::Vector2f& position);
        void Update(float dt, const sf::FloatRect& platformBounds, float screenHeight);
        void Draw(sf::RenderWindow& window) const;

        const BonusContext& GetContext() const { return context; }
        bool HasActiveBonus() const { return activeTime > 0.f; }

    private:
        void Activate(int type);
        std::vector<BonusDrop> drops;
        BonusContext context;
        float activeTime;
        float activeDuration;
    };
}
