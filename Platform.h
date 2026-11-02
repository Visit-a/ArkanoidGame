#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
	class Platform
	{
	public:
		Platform() = default;
		Platform(const sf::Texture& texture, float width, float height, sf::Vector2f position);

		void Move(float shiftX, float screenWidth);
		void Draw(sf::RenderWindow& window);

		const sf::Sprite& GetSprite() const { return sprite; }
		sf::Vector2f GetPosition() const { return sprite.getPosition(); }

	private:
		sf::Sprite sprite;
	};
}