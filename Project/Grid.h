#pragma once
#include "Tiles.h"
#include <vector>

class Grid
{
public:
	Grid() = default;
	~Grid() = default;

	void SetSize(int width, int height);

	std::vector<Tiles> Tiles;

	int map_width;
	int map_height;

	int SpiralTillAvailible(sf::Vector2<int> around_target);

	sf::Vector2<int> SpiralTillCover(sf::Vector2<int> around_target, Direction from);
};