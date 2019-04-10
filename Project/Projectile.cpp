#include "Projectile.h"

Projectile::Projectile(sf::Vector2<float> init_coords, sf::Vector2<float> target_coords, float _speed)
{
	renderer_ptr = Renderer::getRendererInstance();
	game_timer_ptr = GameTimer::getGameTimerInstance();

	setSprite(renderer_ptr->projSprite);
	setColour(sf::Color::Black);
	SetScreenPosition(init_coords.x, init_coords.y);
	speed = _speed;
	target = target_coords;
	renderer_ptr->PushToRenderQueue(this);

	sf::Vector2<float> diff = target_coords - init_coords;

	diff.x *= 0.08f;
	diff.y *= 0.08f;

	direction = diff;
}

void Projectile::Update()
{
	float delta_time = game_timer_ptr->delta_time.asMilliseconds();

	time_elapsed += delta_time;

	if (time_elapsed > 50)
	{
		time_elapsed = 0;
		float x = screen_x_position + (direction.x);
		float y = screen_y_position + (direction.y);

		SetScreenPosition(x, y);

		float x_dis = std::abs(GetScreenPosition().x - target.x);
		float y_dis = std::abs(GetScreenPosition().y - target.y);
		float distance = sqrt((x_dis * x_dis) + (y_dis * y_dis));
		if (distance < 20)
		{
			destroy = true;
		}
	}	
}

bool Projectile::Destroy()
{
	return destroy;
}
