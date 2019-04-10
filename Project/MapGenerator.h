#pragma once
#include "Grid.h"
#include "Renderer.h"
#include "SFML\System\Vector2.hpp"

#include <random>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

class MapGenerator
{
public:
	MapGenerator() = default;
	~MapGenerator() = default;

	Grid* GenerateTerrain(int map_width, int map_height);

	void RunSimulation();

private:

	int checkPassableNeighbors(int row, int col);
	void DetermineCover(int row, int col);

	Grid Grid;

	std::vector<TerrainType> newTiles;

	int map_width;
	int map_height;

	//45 is good
	int start_grass_chance = 45;
	int sim_runs = 3;
};