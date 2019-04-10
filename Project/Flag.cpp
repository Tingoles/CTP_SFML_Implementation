#include "Flag.h"

Flag::Flag()
{
	renderer_ptr = Renderer::getRendererInstance();
}

void Flag::initFlag()
{
	setSprite(renderer_ptr->flagSprite);
	if (team == RED)
	{
		setColour(sf::Color::Red);
	}
	else if (team == BLUE)
	{
		setColour(sf::Color::Blue);
	}
	renderer_ptr->PushToRenderQueue(this);
}

void Flag::Update()
{
}

void Flag::setTeam(Team_Enum _team)
{
	team = _team;
}

Team_Enum Flag::getTeam()
{
	return team;
}