#pragma once
#include "GameObject.h"
#include "Grid.h"
#include "Renderer.h"
#include <math.h>
#include <vector>
#include <random>
#include "GameTimer.h"
#include "Projectile.h"

enum Team_Enum
{
	RED,
	BLUE
};

class Unit : public GameObject
{
public:
	Unit();
	~Unit() = default;

	void Update() override;

	void InitUnit(Grid* TheGrid, int grid_x_pos, int grid_y_pos);

	void setTeam(Team_Enum team);
	Team_Enum getTeam();

	void Move(int target_x, int target_y);

	void MoveToNextPathPoint();

	sf::Vector2<int> getTargetUnitPosition();

	bool getHasPath();

	int unit_ref = 0;

	std::vector<Unit*> Enemies;

	Tiles* current_tile = nullptr;

	bool moving_to_cover = false;
	bool in_cover = false;

	bool in_combat = false;
	bool retreat = false;

	bool alive = true;
private:
	Renderer * renderer_ptr;
	GameTimer* game_timer_ptr;


	float time_since_move = 0;
	float move_after = 500; //milliseconds

	float time_since_check = 0;
	float check_for_enemies_after = 100; //milliseconds

	Team_Enum unit_team;

	std::vector<Projectile*> projectiles;

	sf::Vector2<int> TargetCoords;

	int health = 2;

	bool moving = false;

	float tile_cost_mod = 1;
	float heuristic_cost_mod = 1;

	Grid* grid;

	std::vector<Tiles*> OpenList;
	std::vector<Tiles*> ClosedList;
	std::vector<Tiles*> Path;

	std::vector<Unit*> TargetInRange;
	Unit* current_enemy_target;

	float fire_cooldown = 2000; //milliseconds
	float time_since_fire = 0;

	void checkForEnemies();

	void FireAt(Unit* target);

	void Damage(int damage);
	void Die();
};