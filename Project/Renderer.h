#pragma once
#include "SFML\Graphics.hpp"
#include "GameObject.h"
#include <vector>

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Init();
	bool Render();

	void PushToRenderQueue(GameObject* object);

	sf::Sprite* tileSprite;
	sf::Sprite* unitSprite;
	sf::Sprite* flagSprite;
	sf::Sprite* projSprite;

	static Renderer* getRendererInstance();

	sf::RenderWindow* getWindow();

private:
	//singleton storage
	static Renderer* renderer_instance;

	void LoadSprites();

	sf::RenderWindow window;

	std::vector<std::unique_ptr<GameObject>> render_queue;

	sf::Texture tileTex;
	sf::Texture unitTex;
	sf::Texture flagTex;
	sf::Texture projTex;
};