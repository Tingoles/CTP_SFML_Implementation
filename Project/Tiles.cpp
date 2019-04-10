#include "Tiles.h"

Tiles::Tiles()
{
	cover_from.assign(4, false);
}

void Tiles::Update()
{
}

void Tiles::setType(TerrainType type)
{
	terrainType = type;
}

bool Tiles::getAvailible()
{
	if (terrainType == ROCK)
	{
		return false;
	}
	else if (unit_occupied)
	{
		return false;
	}
	else
	{
		return true;
	}
}
