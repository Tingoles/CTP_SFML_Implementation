#include "Game.h"

void Game::StartGame()
{
	renderer_ptr = Renderer::getRendererInstance();
	game_timer_ptr = GameTimer::getGameTimerInstance();
	map_gen = new MapGenerator;

	GameGrid = map_gen->GenerateTerrain(map_width, map_height);

	team_manager = new TeamManager;
	team_manager->SpawnTeams(GameGrid);
}

void Game::Update()
{
	sf::Event event;
	renderer_ptr->getWindow()->pollEvent(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			team_manager->RedTeam.Squads[0].squad_units[0].Move(15, 15);
		}
	}
	
	team_manager->Update();
}
