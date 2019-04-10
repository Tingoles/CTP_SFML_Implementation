#include "Squad.h"

Squad::Squad()
{
	game_timer_ptr = GameTimer::getGameTimerInstance();
}

Squad::~Squad()
{
	squad_units.clear();
}

void Squad::Update()
{
	if (squad_active)
	{
		time_since_last_order += (float)game_timer_ptr->delta_time.asSeconds();

		combat_found = false;
		units_moving = false;
		units_alive = 0;

		for (int i = 0; i < squad_units.size(); i++)
		{
			if (squad_units[i].alive == true)
			{
				units_alive++;
			}

			squad_units[i].Update();

			if (squad_units[i].retreat == true)
			{
				in_combat = false;
				combat_found = false;
			}
			else
			{
				if (squad_units[i].in_combat)
				{
					combat_found = true;
					combat_unit_index = i;
				}
				else if (squad_units[i].getHasPath())
				{
					units_moving = true;
				}
			}
		}
		if (!combat_found && in_combat)
		{
			in_combat = false;
			orders = SquadOrder::NONE;
		}
		else if (combat_found && !in_combat)
		{
			in_combat = true;
			FindNearestCover(squad_units[in_combat].getTargetUnitPosition(), squad_units[in_combat].getTileCoords());
			combat_found = false;
		}
		if (!units_moving && !in_combat && orders != SquadOrder::DEFENDFLAG)
		{
			orders = SquadOrder::NONE;
		}
		if (in_combat && units_alive < 3)
		{
			setOrder(SquadOrder::DEFENDFLAG);
			for (int i = 0; i < squad_units.size(); i++)
			{
				squad_units[i].retreat = true;
			}
		}
		else if (units_alive == 0)
		{
			squad_active = false;
		}

		if (time_since_last_order > time_between_orders && !in_combat)
		{
			time_since_last_order = 0;
			needs_new_orders = true;
		}
	}
}

void Squad::SpawnSquad(Grid* TheGrid, int squad_size)
{
	//inits the vector
	squad_units.resize(squad_size);

	grid = TheGrid;

	units_alive = squad_size;

	for (int i = 0; i < squad_size; i++)
	{
		bool available_found = false;

		if (team == Team_Enum::RED)
		{
			for (int j = 0; j < grid->map_height; j++)
			{
				for (int k = 0; k < grid->map_width; k++)
				{
					int index = (j * TheGrid->map_width) + k;
					if (grid->Tiles[index].getAvailible())
					{
						squad_units[i].setTeam(team);
						squad_units[i].InitUnit(grid, k, j);
						available_found = true;
						break;
					}
				}
				if (available_found)
				{
					break;
				}
			}
		}

		else if (team == Team_Enum::BLUE)
		{
			for (int j = (TheGrid->map_height - 1); j > 1; j--)
			{
				for (int k = (TheGrid->map_width - 1); k > 1; k--)
				{
					int index = (j * TheGrid->map_width) + k;
					if (TheGrid->Tiles[index].getAvailible())
					{
						squad_units[i].setTeam(team);
						squad_units[i].InitUnit(TheGrid, k, j);
						available_found = true;
						break;
					}
				}
				if (available_found)
				{
					break;
				}
			}
		}
	}
}

void Squad::setTeam(Team_Enum _team)
{
	team = _team;
}

Team_Enum Squad::getTeam()
{
	return team;
}

bool Squad::NeedsOrders()
{
	return needs_new_orders;
}

void Squad::setOrder(SquadOrder new_order)
{
	needs_new_orders = false;
	if (new_order != orders)
	{
		orders = new_order;
		switch (orders)
		{
		case ATTACKFLAG:
			for (int i = 0; i < squad_units.size(); i++)
			{
				squad_units[i].Move(target_flag.getTileCoords().x, target_flag.getTileCoords().y);
			}
			break;
		case DEFENDFLAG:
			FindNearestCover(target_flag.getTileCoords(), own_flag.getTileCoords());
			break;
		case NONE:
			break;
		}
	}
}

SquadOrder Squad::getOrder()
{
	return orders;
}

void Squad::FindNearestCover(sf::Vector2<int> FromGridPos, sf::Vector2<int> AroundGridPos)
{
	//nearest cover to the passed in position
	float x_dis = (float)AroundGridPos.x - (float)FromGridPos.x;
	float y_dis = (float)AroundGridPos.y - (float)FromGridPos.y;

	std::vector<sf::Vector2<int>> CoverPositions;

	Direction cover_from;

	if (std::abs(x_dis) > std::abs(y_dis))
	{
		//cover from horizontal
		if (x_dis > 0)
		{
			cover_from = Direction::EAST;
		}
		else
		{
			cover_from = Direction::WEST;
		}
	}
	else
	{
		//cover from vertical
		if (y_dis > 0)
		{
			cover_from = Direction::NORTH;
		}
		else
		{
			cover_from = Direction::SOUTH;
		}
	}

	int safety = 0;
	while (CoverPositions.size() <= squad_units.size())
	{
		sf::Vector2<int> cover_pos = grid->SpiralTillCover(AroundGridPos, cover_from);
		CoverPositions.push_back(cover_pos);
		grid->Tiles[(grid->map_width * cover_pos.y) + cover_pos.x].cover_marked = true;

		/*safety++;
		if (safety > 20)
		{
			return;
		}*/
	}


	for (int i = 0; i < squad_units.size(); i++)
	{
		if (squad_units[i].moving_to_cover == false && squad_units[i].in_cover == false)
		{
			squad_units[i].Move(CoverPositions.back().x, CoverPositions.back().y);
			squad_units[i].moving_to_cover = true;
			CoverPositions.pop_back();
		}
	}
}