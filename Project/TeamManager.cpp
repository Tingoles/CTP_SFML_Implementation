#include "TeamManager.h"

void TeamManager::SpawnTeams(Grid* TheGrid)
{
	RedTeam.setTeam(RED);
	BlueTeam.setTeam(BLUE);

	RedTeam.SpawnSquads(TheGrid, squad_size, no_red_units, true);
	BlueTeam.SpawnSquads(TheGrid, squad_size, no_blue_units, true);

	RedTeam.giveDataOnOpponent(&BlueTeam);
	BlueTeam.giveDataOnOpponent(&RedTeam);

	int unit_ref = 0;
	for (Squad &squad : RedTeam.Squads)
	{
		for (Unit &unit : squad.squad_units)
		{
			unit.unit_ref = unit_ref;
			unit_ref++;
		}
	}
	for (Squad &squad : BlueTeam.Squads)
	{
		for (Unit &unit : squad.squad_units)
		{
			unit.unit_ref = unit_ref;
			unit_ref++;
		}
	}
}

void TeamManager::Update()
{
	RedTeam.Update();
	BlueTeam.Update();
}
