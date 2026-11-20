#pragma once
#include <memory>
#include <string>

namespace Patterns
{
    struct EnemyPosition
    {
        float x;
        float y;
        EnemyPosition(float px = 0.f, float py = 0.f) : x(px), y(py) {}
    };

    class Enemy
    {
    public:
        Enemy(const EnemyPosition& pos, float moveSpeed, int hp)
            : position(pos), speed(moveSpeed), health(hp) {}
        virtual ~Enemy() {}
        virtual std::unique_ptr<Enemy> Clone() const = 0;
        virtual const char* Type() const = 0;

        void SetPosition(const EnemyPosition& pos) { position = pos; }
        EnemyPosition GetPosition() const { return position; }
        float GetSpeed() const { return speed; }
        int GetHealth() const { return health; }

    protected:
        EnemyPosition position;
        float speed;
        int health;
    };

    class BasicEnemy : public Enemy
    {
    public:
        explicit BasicEnemy(const EnemyPosition& p = EnemyPosition())
            : Enemy(p, 100.f, 1) {}
        std::unique_ptr<Enemy> Clone() const override
        { return std::unique_ptr<Enemy>(new BasicEnemy(*this)); }
        const char* Type() const override { return "Обычный"; }
    };

    class FastEnemy : public Enemy
    {
    public:
        explicit FastEnemy(const EnemyPosition& p = EnemyPosition())
            : Enemy(p, 220.f, 1) {}
        std::unique_ptr<Enemy> Clone() const override
        { return std::unique_ptr<Enemy>(new FastEnemy(*this)); }
        const char* Type() const override { return "Быстрый"; }
    };

    class TankEnemy : public Enemy
    {
    public:
        explicit TankEnemy(const EnemyPosition& p = EnemyPosition())
            : Enemy(p, 55.f, 5) {}
        std::unique_ptr<Enemy> Clone() const override
        { return std::unique_ptr<Enemy>(new TankEnemy(*this)); }
        const char* Type() const override { return "Бронированный"; }
    };
}
