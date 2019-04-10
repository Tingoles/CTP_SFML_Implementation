#include "Grid.h"

void Grid::SetSize(int width, int height)
{
	map_width = width;
	map_height = height;
}

int Grid::SpiralTillAvailible(sf::Vector2<int> around_target)
{
	int range = 1;
	int around_index = (around_target.y * map_width) + around_target.x;

	while (!(Tiles[around_index].getAvailible()))
	{
		for (int i = -range; i < (1 + range); i++)
		{
			for (int j = -range; j < (1 + range); j++)
			{
				int x = around_target.x + i;
				int y = around_target.y + j;

				if (x < 0 || y < 0 || x >= map_width || y >= map_height) {} //is off map?

				else
				{
					int target_tile_ref = (y * map_width) + x;

					if (Tiles[target_tile_ref].getAvailible())
					{
						return target_tile_ref;
					}
				}
			}
		}

		range++;

		if (range > 4)
		{
			return 0;
		}
	}
}

sf::Vector2<int> Grid::SpiralTillCover(sf::Vector2<int> around_target, Direction from)
{
	int x = around_target.x;
	int y = around_target.y;
	int x_increment = 1;
	int y_increment = 0;

	int x_diff = 0;
	int y_diff = 0;


	int range = 1;

	int radius_to_check = 40;
	int max_iterator = radius_to_check * radius_to_check;

	for (int i = 0; i < max_iterator; i++)
	{
		//if on map
		if (!(x < 0 || y < 0 || x >= map_width || y >= map_height))
		{
			int target_tile_ref = (y * map_width) + x;
			sf::Vector2<int> debug = Tiles[target_tile_ref].getTileCoords();

			if (Tiles[target_tile_ref].getAvailible() && Tiles[target_tile_ref].cover_marked == false && Tiles[target_tile_ref].cover_from[from] == true)
			{				
				return Tiles[target_tile_ref].getTileCoords();
			}
		}

		x_diff = x - around_target.x;
		y_diff = y - around_target.y;

		if (x_diff == range && x_increment != 0)
		{
			x_increment = 0;
			y_increment = -1;
		}
		else if (y_diff == -range && y_increment != 0)
		{
			x_increment = -1;
			y_increment = 0;
		}
		else if (x_diff == -range && x_increment != 0)
		{
			x_increment = 0;
			y_increment = 1;
		}
		else if (y_diff == range && y_increment != 0)
		{
			x_increment = 1;
			y_increment = 0;
			range++;
		}

		int stahp = 0;
		x += x_increment;
		y += y_increment;
	}

	return around_target;
}


	//int range = 1;
	//int around_index = (around_target.y * map_width) + around_target.x;

	//while (!(Tiles[around_index].getAvailible()))
	//{
	//	for (int i = -range; i < (1 + range); i++)
	//	{
	//		for (int j = -range; j < (1 + range); j++)
	//		{
	//			int x = around_target.x + i;
	//			int y = around_target.y + j;

	//			//if not off map
	//			if (!(x < 0 || y < 0 || x >= map_width || y >= map_height))
	//			{
	//				int target_tile_ref = (y * map_width) + x;

	//				if (Tiles[target_tile_ref].getAvailible() && Tiles[target_tile_ref].cover_marked == false && Tiles[target_tile_ref].cover_from[from] == true)
	//				{
	//					return Tiles[target_tile_ref].getTileCoords();
	//				}
	//			}
	//		}
	//	}

	//	range++;

	//	if (range > 20)
	//	{
	//		return around_target;
	//	}
	//}

//
