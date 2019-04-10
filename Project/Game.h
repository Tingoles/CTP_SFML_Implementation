#pragma once
#include <vector>
#include "MapGenerator.h"
#include "Renderer.h"
#include "Tiles.h"
#include "TeamManager.h"

class Game
{
public:
	Game() = default;
	~Game() = default;

	void StartGame();

	void Update();

	int map_height = 100;
	int map_width = 100;

	bool game_over;

private:
	Grid* GameGrid;
	MapGenerator* map_gen = nullptr;
	Renderer* renderer_ptr;
	TeamManager* team_manager;
	GameTimer* game_timer_ptr;
};