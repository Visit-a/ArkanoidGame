#pragma once
#include <vector>
#include <algorithm>

namespace SnakeGame
{
    class IScoreObserver
    {
    public:
        virtual ~IScoreObserver() {}
        virtual void OnScoreChanged(int newScore) = 0;
    };

    class ScoreSubject
    {
    public:
        explicit ScoreSubject(int initialScore = 0) : score(initialScore) {}

        void Attach(IScoreObserver* observer)
        {
            if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end())
                observers.push_back(observer);
        }

        void Detach(IScoreObserver* observer)
        {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void AddPoints(int points)
        {
            if (points <= 0) return;
            score += points;
            Notify();
        }

        int GetScore() const { return score; }

    private:
        void Notify()
        {
            for (IScoreObserver* observer : observers)
                if (observer) observer->OnScoreChanged(score);
        }

        int score;
        std::vector<IScoreObserver*> observers;
    };
}
