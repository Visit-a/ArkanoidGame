#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
	class Ball
	{
	public:
		Ball() = default;
		Ball(const sf::Texture& texture, float radius, sf::Vector2f position);

		void SetVelocity(sf::Vector2f v) { velocity = v; }
		sf::Vector2f GetVelocity() const { return velocity; }

		void Move(float timeDelta);
		void Draw(sf::RenderWindow& window);

		const sf::Sprite& GetSprite() const { return sprite; }
		sf::FloatRect GetGlobalBounds() const { return sprite.getGlobalBounds(); }
		sf::Vector2f GetPosition() const { return sprite.getPosition(); }
		void SetPosition(sf::Vector2f p) { sprite.setPosition(p); }

	private:
		sf::Sprite sprite;
		sf::Vector2f velocity{ 0.f, 0.f };
	};
}