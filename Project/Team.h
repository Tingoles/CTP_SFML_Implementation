#pragma once
#include "Squad.h"
#include <vector>

class Team
{
public:
	Team() = default;
	~Team() = default;

	void Update();

	void SpawnSquads(Grid* TheGrid, int squad_size, int no_units, bool place_flags);

	void setTeam(Team_Enum team);
	Team_Enum getTeam();

	void giveDataOnOpponent(Team* opponent);

	SquadOrder OrderRequest(Squad squad);

	std::vector<Squad> Squads;
	Flag own_flag;

	Flag enemy_flag;
	std::vector<Unit*> enemy_units;

private:
	Team_Enum team;

	int squads_active;

	int map_width;
	int map_height;

	int squads_defending;
	int squads_attacking;
};