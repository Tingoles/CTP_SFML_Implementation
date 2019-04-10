#pragma once
#include "Unit.h"
#include "Flag.h"
#include <vector>

enum SquadOrder
{
	ATTACKFLAG,
	DEFENDFLAG,
	NONE
};

class Squad
{
public:
	Squad();
	~Squad();

	void Update();

	void SpawnSquad(Grid* TheGrid, int squad_size);

	void setTeam(Team_Enum team);
	Team_Enum getTeam();

	std::vector<Unit> squad_units;

	Flag own_flag;
	Flag target_flag;

	bool NeedsOrders();
	void setOrder(SquadOrder new_order);

	SquadOrder getOrder();

	void FindNearestCover(sf::Vector2<int> FromGridPos, sf::Vector2<int> AroundGridPos);

	bool squad_active = true;

private:
	Team_Enum team;

	SquadOrder orders = NONE;

	GameTimer* game_timer_ptr;

	Grid* grid;

	int units_alive;

	bool needs_new_orders = false;
	bool in_combat = false;
	bool combat_found = false;
	int combat_unit_index = -1;

	bool units_moving = false;

	float time_since_last_order = 8;
	float time_between_orders = 12;
};