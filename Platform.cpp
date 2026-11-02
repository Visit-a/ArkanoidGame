#include "Platform.h"
#include "Sprite.h"

namespace SnakeGame
{
	Platform::Platform(const sf::Texture& texture, float width, float height, sf::Vector2f position)
	{
		InitSprite(sprite, width, height, texture);
		sprite.setPosition(position);
	}

	void Platform::Move(float shiftX, float screenWidth)
	{
		sf::Vector2f pos = sprite.getPosition();
		pos.x += shiftX;

		float halfWidth = sprite.getGlobalBounds().width / 2.f;
		if (pos.x - halfWidth < 0.f) pos.x = halfWidth;
		if (pos.x + halfWidth > screenWidth) pos.x = screenWidth - halfWidth;

		sprite.setPosition(pos);
	}

	void Platform::Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
}