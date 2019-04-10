#include "Unit.h"

Unit::Unit()
{
	renderer_ptr = Renderer::getRendererInstance();
	game_timer_ptr = GameTimer::getGameTimerInstance();
}

void Unit::Update()
{
	if (alive)
	{
		float delta_time = (float)game_timer_ptr->delta_time.asMilliseconds();

		time_since_move += delta_time;
		time_since_check += delta_time;
		time_since_fire += delta_time;

		if (time_since_move > move_after)
		{
			time_since_move = 0;

			if (Path.size() > 0)
			{
				MoveToNextPathPoint();
			}
			else if (Path.size() == 0)
			{
				if (moving_to_cover)
				{
					moving_to_cover = false;
					in_cover = true;
				}
				if (retreat)
				{
					retreat = false;
				}
			}
		}

		if (time_since_check > check_for_enemies_after)
		{
			time_since_check = 0;
			checkForEnemies();
		}

		if (TargetInRange.size() > 0)
		{
			if (time_since_fire > fire_cooldown)
			{
				std::sort(TargetInRange.begin(), TargetInRange.end());
				time_since_fire = 0;
				current_enemy_target = TargetInRange[0];
				FireAt(current_enemy_target);
			}
		}
	}


	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->Update();
		if (projectiles[i]->Destroy())
		{
			projectiles[i]->setColour(sf::Color::Transparent);
			//projectiles.erase(projectiles.begin() + i);
		}
	}
}

void Unit::InitUnit(Grid* _TheGrid, int x_pos, int y_pos)
{
	grid = _TheGrid;

	//set sprite and pos
	setSprite(renderer_ptr->unitSprite);
	SetPosition(sf::Vector2<int>(x_pos, y_pos));

	int tile_ref = (y_pos * grid->map_width) + x_pos;
	grid->Tiles[tile_ref].unit_occupied = true;
	current_tile = &grid->Tiles[tile_ref];

	if (unit_team == RED)
	{
		setColour(sf::Color::Red);
	}
	else if (unit_team == BLUE)
	{
		setColour(sf::Color::Blue);
	}

	renderer_ptr->PushToRenderQueue(this);
}

void Unit::setTeam(Team_Enum team)
{
	unit_team = team;
}

Team_Enum Unit::getTeam()
{
	return unit_team;
}

void Unit::Move(int target_x, int target_y)
{
	if (moving_to_cover || (in_cover && in_combat) || retreat)
	{
		//then let this finish
		return;
	}

	TargetCoords.x = target_x;
	TargetCoords.y = target_y;

	int current_tile_ref = (TileCoords.y * grid->map_width) + TileCoords.x;
	int target_tile_ref = (target_y * grid->map_width) + target_x;

	//Changes the target if not availible
	if (!grid->Tiles[target_tile_ref].getAvailible())
	{
		target_tile_ref = grid->SpiralTillAvailible(TargetCoords);
		TargetCoords = grid->Tiles[target_tile_ref].getTileCoords();
	}

	grid->Tiles[current_tile_ref].unit_occupied = false;
	grid->Tiles[target_tile_ref].unit_occupied = true;
	grid->Tiles[target_tile_ref].cover_marked = false;

	float tile_cost;
	float heuristic_cost;
	float total_cost;
	int list_index = 0;

	Tiles* OriginTile = &grid->Tiles[current_tile_ref];

	if (Path.size() > 0)
	{
		Path.back()->unit_occupied = false;
		Path.back()->planned_move = false;
	}

	OpenList.clear();
	ClosedList.clear();
	Path.clear();

	//OpenList.push_back(OriginTile);

	int safety_iterator = 0;

	while (OriginTile->getTileCoords() != TargetCoords)
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int neighbour_x = OriginTile->getTileCoords().x + i;
				int neighbour_y = OriginTile->getTileCoords().y + j;
				int tiles_index = (neighbour_y * grid->map_width) + neighbour_x;

				if (i == 0 && j == 0) {} //this tile
				else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= grid->map_width || neighbour_y >= grid->map_height) {} //checks if neighbor is off the map
				else if (grid->Tiles[tiles_index].visited == true) {} //then we don't want to visit again, checks essentially if on closed list
				else if (grid->Tiles[tiles_index].terrainType == GRASS) //finally, if not obstacle
				{
					Tiles* neigboring_tile = &grid->Tiles[tiles_index];

					//Calc Tile Cost
					if (j == 0 || i == 0)
					{
						tile_cost = OriginTile->tile_cost + (1.0f * tile_cost_mod);
					}
					else
					{
						//sqrt 2
						tile_cost = OriginTile->tile_cost + (1.41f * tile_cost_mod);
					}
					float x_dis = std::abs(TargetCoords.x - neighbour_x);
					float y_dis = std::abs(TargetCoords.y - neighbour_y);

					heuristic_cost = sqrt((x_dis * x_dis) + (y_dis * y_dis)) * heuristic_cost_mod;
					total_cost = tile_cost + heuristic_cost;

					if (neigboring_tile->parent_tile == nullptr)
					{
						neigboring_tile->parent_tile = OriginTile;

						neigboring_tile->tile_cost = tile_cost;

						neigboring_tile->total_cost = tile_cost + heuristic_cost;

						//neigboring_tile->setColour(sf::Color::Red);

						OpenList.push_back(neigboring_tile);
					}
					else
					{
						if (neigboring_tile->tile_cost > tile_cost)
						{
							neigboring_tile->tile_cost = tile_cost;
							neigboring_tile->parent_tile = OriginTile;
						}
					}
				}
			}
		}
		OriginTile->visited = true;

		//add to closed before we chose the next origin
		ClosedList.push_back(OriginTile);

		float lowest_cost = 0;
		for (int i = 0; i < OpenList.size(); i++)
		{
			if (OpenList[i]->total_cost < lowest_cost || lowest_cost == 0)
			{
				lowest_cost = OpenList[i]->total_cost;
				list_index = i;
			}
		}

		if (OpenList.size() < 1)
		{
			//shouldn't happen
			break;
		}
		OriginTile = OpenList[list_index];

		//remove from open list as we're about to check it
		OpenList.erase(OpenList.begin() + list_index);

		safety_iterator++;
		if (safety_iterator > 5000)
		{
			break;
		}
	}
	//add the destination to the closed list so it gets cleared up
	ClosedList.push_back(OriginTile);

	//construct dat path
	Tiles* OnPath = OriginTile;

	while (OnPath->parent_tile != nullptr)
	{
		//OnPath->setColour(sf::Color::Yellow);
		Path.push_back(OnPath);
		OnPath = OnPath->parent_tile;
	}
	//std::reverse(Path.begin(), Path.end());

	for (int i = 0; i < OpenList.size(); i++)
	{
		OpenList[i]->parent_tile = nullptr;
	}
	for (int i = 0; i < ClosedList.size(); i++)
	{
		ClosedList[i]->parent_tile = nullptr;
        ClosedList[i]->visited = false;
	}
	OpenList.clear();
	ClosedList.clear();
}

void Unit::MoveToNextPathPoint()
{
	Tiles* move_to = Path.back();
	if (move_to->planned_unit_ref == unit_ref)
	{
		Path.pop_back();
		SetPosition(move_to->getTileCoords());
		move_to->planned_move = false;
	}
	
	if (Path.size() > 0)
	{
		Tiles* next_move_to = Path.back();
		if (next_move_to->planned_move == false)
		{
			next_move_to->planned_unit_ref = unit_ref;
			next_move_to->planned_move = true;
		}
	}
}

sf::Vector2<int> Unit::getTargetUnitPosition()
{
	if (TargetInRange.size() > 0)
	{
		return TargetInRange[0]->getTileCoords();
	}
	else
	{
		return TargetCoords;
	}
}

bool Unit::getHasPath()
{
	if (Path.size() > 0 && alive)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Unit::checkForEnemies()
{
	TargetInRange.clear();
	for (Unit* enemy : Enemies)
	{
		if (enemy->alive)
		{
			float x_dis = (float)std::abs(getTileCoords().x - enemy->getTileCoords().x);
			float y_dis = (float)std::abs(getTileCoords().y - enemy->getTileCoords().y);
			float distance = sqrt((x_dis * x_dis) + (y_dis * y_dis));

			if (distance < 30)
			{
				TargetInRange.push_back(enemy);
			}
		}
	}
	if (TargetInRange.size() > 0)
	{
		in_combat = true;
	}
	else
	{
		in_combat = false;
	}
}

void Unit::FireAt(Unit * target)
{
	int hit_chance = 50;
	if (target->in_cover)
	{
		hit_chance = hit_chance / 2;
	}

	std::random_device random;
	std::mt19937 generator(random());
	std::uniform_int_distribution<int> number_distribution(0, 100);

	projectiles.push_back(new Projectile(GetScreenPosition(), target->GetScreenPosition(), 5));

	int hit = number_distribution(generator);
	if (hit < hit_chance)
	{
		target->Damage(1);
		if (target->alive == false)
		{
			checkForEnemies();
		}
	}
}

void Unit::Damage(int damage)
{
	health = health - damage;
	if (health < 1)
	{
		Die();
	}
}

void Unit::Die()
{
	alive = false;
	in_combat = false;
	if (Path.size() > 0)
	{
		Path.back()->planned_move = false;
		Path[0]->unit_occupied = false;
		Path.clear();
	}

	setColour(sf::Color(0,0,0, 100));
}
