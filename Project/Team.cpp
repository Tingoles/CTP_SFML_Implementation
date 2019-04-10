#include "Team.h"

void Team::SpawnSquads(Grid* TheGrid, int squad_size, int no_units, bool place_flags)
{
	int no_squads = no_units / squad_size;

	squads_active = no_squads;

	//inits
	Squads.resize(no_squads);

	for (int i = 0; i < no_squads; i++)
	{
		Squads[i].setTeam(team);
		Squads[i].SpawnSquad(TheGrid, squad_size);
	}

	int flag_placement = no_squads / 2;

	own_flag.setTeam(team);
	own_flag.initFlag();
	own_flag.SetPosition(Squads[flag_placement].squad_units[0].getTileCoords());
}

void Team::Update()
{
	squads_active = 0;
	for (int i = 0; i < Squads.size(); i++)
	{
		if (Squads[i].squad_active)
		{
			squads_active++;
		}
	}

	for (int i = 0; i < Squads.size(); i++)
	{
		Squads[i].Update();

		if (Squads[i].NeedsOrders())
		{
			Squads[i].setOrder(OrderRequest(Squads[i]));
		}
	}
}

void Team::setTeam(Team_Enum _team)
{
	team = _team;
}

Team_Enum Team::getTeam()
{
	return team;
}

void Team::giveDataOnOpponent(Team* opponent)
{
	enemy_flag = opponent->own_flag;

	for (int i = 0; i < opponent->Squads.size(); i++)
	{
		for (int j = 0; j < opponent->Squads[i].squad_units.size(); j++)
		{
			enemy_units.push_back(&(opponent->Squads[i].squad_units[j]));
		}
	}

	for (Squad &squad : Squads)
	{
		for (Unit &unit : squad.squad_units)
		{
			for (Unit* &enemy_unit : enemy_units)
			{
				unit.Enemies.push_back(enemy_unit);
			}
		}
		squad.target_flag = enemy_flag;
		squad.own_flag = own_flag;
	}
}

SquadOrder Team::OrderRequest(Squad squad)
{
	SquadOrder old_order = squad.getOrder();
	//attack and defend variables for init
	if (old_order == SquadOrder::NONE)
	{
		if (squads_defending > 0)
		{
			squads_attacking++;
			return ATTACKFLAG;
		}
		else
		{
			squads_defending++;
			return DEFENDFLAG;
		}
	}

	if (squads_active == 1)
	{
		return ATTACKFLAG;
	}

	else if (old_order == SquadOrder::DEFENDFLAG)
	{
		return DEFENDFLAG;
	}
	return ATTACKFLAG;
}
