#include "Ball.h"
#include "Sprite.h"

namespace SnakeGame
{
	Ball::Ball(const sf::Texture& texture, float radius, sf::Vector2f position)
	{
		InitSprite(sprite, radius * 2.f, radius * 2.f, texture);
		sprite.setPosition(position);
	}

	void Ball::Move(float timeDelta)
	{
		sprite.setPosition(sprite.getPosition() + velocity * timeDelta);
	}

	void Ball::Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
}