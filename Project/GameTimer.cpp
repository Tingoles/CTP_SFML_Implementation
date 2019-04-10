#include "GameTimer.h"

GameTimer* GameTimer::game_timer_instance;

void GameTimer::Tick()
{
	delta_time = clock.restart();
}

GameTimer * GameTimer::getGameTimerInstance()
{
	if (game_timer_instance == 0)
	{
		game_timer_instance = new GameTimer();
	}

	return game_timer_instance;
}
