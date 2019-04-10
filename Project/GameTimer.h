#pragma once
#include "SFML/System/Clock.hpp"

class GameTimer
{
public:
	GameTimer() = default;
	~GameTimer() = default;

	void Tick();

	sf::Clock clock;
	sf::Time delta_time;

	static GameTimer* getGameTimerInstance();

private:
	static GameTimer* game_timer_instance;
};