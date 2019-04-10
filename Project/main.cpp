#include "Game.h"
#include "GameTimer.h"
#include "Renderer.h"
#include <iostream>

int main()
{
	Renderer* renderer = Renderer::getRendererInstance();
	renderer->Init();

	Game game;
	game.StartGame();

	GameTimer* game_timer = GameTimer::getGameTimerInstance();

	while (!(game.game_over))
	{
		game_timer->Tick();

		game.Update();

		//if we shut the window this will close the program
		if (!renderer->Render())
		{
			break;
		}
	}

	return 0;
}