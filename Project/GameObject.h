#pragma once
#include "SFML\Graphics.hpp"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	virtual void Update() = 0;

	void SetPosition(sf::Vector2<int> GridPos);
	sf::Vector2<int> getTileCoords();
	void GetScreenPosition(float &x, float &y);
	sf::Vector2<float> GetScreenPosition();

	void setSprite(sf::Sprite* sprite);
	sf::Sprite getSprite();

	void setColour(sf::Color colour);

protected:
	void SetScreenPosition(float x, float y);

	float screen_x_position;
	float screen_y_position;

	sf::Vector2<int> TileCoords;

	sf::Sprite sprite;
	sf::Color colour;
};