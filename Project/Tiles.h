#pragma once
#include "GameObject.h"
#include "SFML\System\Vector2.hpp"

enum TerrainType
{
	GRASS = 0,
	ROCK = 1
};

enum Direction
{
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
};

class Tiles : public GameObject
{
public:
	Tiles();
	~Tiles() = default;

	void Update() override;

	TerrainType terrainType;

	bool unit_occupied = false;
	bool planned_move = false;
	int planned_unit_ref = -1;

	//pathfinding
	int tile_ref;
	Tiles* parent_tile = nullptr;
	float tile_cost;
	float total_cost;
	bool visited = false;

	void setType(TerrainType type);

	bool getAvailible();

	std::vector<bool> cover_from;

	bool cover_marked = false;
};