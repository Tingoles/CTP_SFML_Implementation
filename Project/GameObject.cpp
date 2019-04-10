#include "GameObject.h"

//converts grid pos to screen pos
void GameObject::SetPosition(sf::Vector2<int> GridPos)
{
	TileCoords = GridPos;

	int offset = 10;
	sf::Vector2u sprite_size = sprite.getTexture()->getSize();
	SetScreenPosition((TileCoords.x * sprite_size.x) + offset,
		(TileCoords.y * sprite_size.y) + offset);
}

sf::Vector2<int> GameObject::getTileCoords()
{
	return TileCoords;
}

void GameObject::SetScreenPosition(float x, float y)
{
	screen_x_position = x;
	screen_y_position = y;

	sprite.setPosition(screen_x_position, screen_y_position);
}

void GameObject::GetScreenPosition(float & x, float & y)
{
	x = screen_x_position;
	y = screen_y_position;
}

sf::Vector2<float> GameObject::GetScreenPosition()
{
	return sf::Vector2<float>(screen_x_position, screen_y_position);
}

void GameObject::setSprite(sf::Sprite * _sprite)
{
	sprite = *_sprite;
}

sf::Sprite GameObject::getSprite()
{
	return sprite;
}

void GameObject::setColour(sf::Color colour)
{
	sprite.setColor(colour);
}