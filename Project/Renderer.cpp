#include "Renderer.h"

Renderer* Renderer::renderer_instance;

void Renderer::Init()
{
	window.create(sf::VideoMode(1600, 900), "Huw CTP!");

	LoadSprites();
}

void Renderer::LoadSprites()
{
	tileTex.loadFromFile("Tile.png");
	tileSprite = new sf::Sprite;
	tileSprite->setTexture(tileTex);

	unitTex.loadFromFile("Circle.png");
	unitSprite = new sf::Sprite;
	unitSprite->setTexture(unitTex);

	flagTex.loadFromFile("FlagSprite.png");
	flagSprite = new sf::Sprite;
	flagSprite->setTexture(flagTex);

	projTex.loadFromFile("Projectile.png");
	projSprite = new sf::Sprite;
	projSprite->setTexture(projTex);
}

bool Renderer::Render()
{
	if (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return false;
			}
		}

		window.clear(sf::Color::Black);

		for (int i = 0; i < render_queue.size(); i++)
		{
			sf::Sprite sprite = (render_queue[i]->getSprite());
			window.draw(sprite);
		}

		window.display();
		return true;
	}
	else
	{
		return false;
	}
}


void Renderer::PushToRenderQueue(GameObject* object)
{
	render_queue.push_back(std::unique_ptr<GameObject>(object));
}

Renderer* Renderer::getRendererInstance()
{
	if (renderer_instance == 0)
	{
		renderer_instance = new Renderer();
	}

	return renderer_instance;
}

sf::RenderWindow * Renderer::getWindow()
{
	return &window;
}
