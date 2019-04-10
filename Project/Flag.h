#pragma once
#include "GameObject.h"
#include "Unit.h"

class Flag : public GameObject
{
public:
	Flag();
	~Flag() = default;

	void initFlag();

	void Update() override;

	bool taken = false;

	void setTeam(Team_Enum team);
	Team_Enum getTeam();

private:
	Renderer * renderer_ptr;
	Team_Enum team;
};