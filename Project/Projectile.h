#pragma once
#include "Renderer.h"
#include "GameTimer.h"
#include <math.h>

class Projectile : public GameObject
{
public:
	Projectile(sf::Vector2<float> init_coords, sf::Vector2<float> target_coords, float speed);
	~Projectile() = default;

	void Update() override;

	bool Destroy();

private:
	Renderer* renderer_ptr;
	GameTimer* game_timer_ptr;

	float time_elapsed = 0;
	float speed;
	sf::Vector2<float> direction;

	sf::Vector2<float> target;

	bool destroy = false;
};