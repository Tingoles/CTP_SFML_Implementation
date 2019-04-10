#include "MapGenerator.h"

Grid* MapGenerator::GenerateTerrain(int width, int height)
{
	Renderer* renderer = Renderer::getRendererInstance();

	map_width = width;
	map_height = height;

	Grid.SetSize(map_width, map_height);
	Grid.Tiles.reserve(map_width*map_height);

	std::random_device random;
	std::mt19937 generator(random());
	std::uniform_int_distribution<int> number_distribution(0, 100);

	//for runsim
	newTiles.reserve(map_width*map_height);
	for (int i = 0; i < (map_width*map_height); i++)
	{
		newTiles.push_back(GRASS);
	}

	//random each tile
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			int tile_ref = (i * map_width) + j;

			Tiles Current_Tile;
			Current_Tile.tile_ref = tile_ref;

			//set sprite and pos
			Current_Tile.setSprite(renderer->tileSprite);
			Current_Tile.SetPosition(sf::Vector2<int>(j, i));

			int Random = number_distribution(generator);
			//Random passable/impassable
			if (Random > start_grass_chance)
			{
				Current_Tile.setType(ROCK);
			}
			else
			{
				Current_Tile.setType(GRASS);
			}

			Grid.Tiles.push_back(Current_Tile);
		}
	}

	for (int i = 0; i < sim_runs; i++)
	{
		RunSimulation();
	}

	//setting tile colour and adding tiles to the render queue
	for (int i = 0; i < Grid.Tiles.size(); i++)
	{
		if (Grid.Tiles[i].terrainType == GRASS)
		{
			Grid.Tiles[i].setColour(sf::Color::Green);
			
			DetermineCover(Grid.Tiles[i].getTileCoords().x, Grid.Tiles[i].getTileCoords().y);
		}
		else
		{
			//it's grey
			int adj_rock = (8 - checkPassableNeighbors(Grid.Tiles[i].getTileCoords().x, Grid.Tiles[i].getTileCoords().y));

			int colour = 192 - (adj_rock * 10);

			Grid.Tiles[i].setColour(sf::Color(colour, colour, colour));
		}
		renderer->PushToRenderQueue(&Grid.Tiles[i]);
	}

	return &Grid;
}

void MapGenerator::RunSimulation()
{
	//Based off Conways Game of Life
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			int tile_ref = (i * map_width) + j;
			Tiles Current_Tile = Grid.Tiles[tile_ref];

			int num = checkPassableNeighbors(j, i);

			if (Current_Tile.terrainType == GRASS)
			{
				if (num < 3)
				{
					newTiles[tile_ref] = ROCK;
				}
				else
				{
					newTiles[tile_ref] = GRASS;
				}
			}
			else //if rock
			{
				if (num > 3)
				{
					newTiles[tile_ref] = GRASS;
				}
				else
				{
					newTiles[tile_ref] = ROCK;
				}
			}
		}
	}

	//apply
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			int tile_ref = (i * map_width) + j;
			Grid.Tiles[tile_ref].setType(newTiles[tile_ref]);
		}
	}
}

int MapGenerator::checkPassableNeighbors(int row, int col)
{
	int no_passable_neighbors = 0;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			int neighbour_x = row + i;
			int neighbour_y = col + j;
			//If we're looking at the middle point
			if (i == 0 && j == 0)
			{
				//Do nothing, is us
			}
			//if off map
			else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= map_width || neighbour_y >= map_height)
			{
				//no_passable_neighbors++;
			}
			//Otherwise, normal check
			else if (Grid.Tiles[(neighbour_y * map_width) + neighbour_x].terrainType == GRASS)
			{
				no_passable_neighbors++;
			}
		}
	}

	return no_passable_neighbors;
}

void MapGenerator::DetermineCover(int row, int col)
{
	int index = (col * map_width) + row;

	//North
	int neighbour_x = row;
	int neighbour_y = col - 1;
	int neighbour_index = (neighbour_y * map_width) + neighbour_x;
	//make sure not off map
	if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= map_width || neighbour_y >= map_height){}
	else if (Grid.Tiles[neighbour_index].terrainType == ROCK)
	{
		Grid.Tiles[index].cover_from[NORTH] = true;
	}

	//South
	neighbour_x = row;
	neighbour_y = col + 1;
	neighbour_index = (neighbour_y * map_width) + neighbour_x;
	//make sure not off map
	if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= map_width || neighbour_y >= map_height) {}
	else if (Grid.Tiles[neighbour_index].terrainType == ROCK)
	{
		Grid.Tiles[index].cover_from[SOUTH] = true;
	}

	//East
	neighbour_x = row - 1;
	neighbour_y = col;
	neighbour_index = (neighbour_y * map_width) + neighbour_x;
	//make sure not off map
	if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= map_width || neighbour_y >= map_height) {}
	else if (Grid.Tiles[neighbour_index].terrainType == ROCK)
	{
		Grid.Tiles[index].cover_from[EAST] = true;
	}

	//West
	neighbour_x = row + 1;
	neighbour_y = col;
	neighbour_index = (neighbour_y * map_width) + neighbour_x;
	//make sure not off map
	if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= map_width || neighbour_y >= map_height) {}
	else if (Grid.Tiles[neighbour_index].terrainType == ROCK)
	{
		Grid.Tiles[index].cover_from[WEST] = true;
	}
}
