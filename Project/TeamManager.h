#pragma once
#include "Team.h"

class TeamManager
{
public:
	TeamManager() = default;
	~TeamManager() = default;

	void SpawnTeams(Grid* TheGrid);
	void Update();

	Team RedTeam;
	Team BlueTeam;

	std::vector<Unit*> all_units;

private:
	int no_red_units = 12;
	int no_blue_units = 12;

	int squad_size = 4;
};