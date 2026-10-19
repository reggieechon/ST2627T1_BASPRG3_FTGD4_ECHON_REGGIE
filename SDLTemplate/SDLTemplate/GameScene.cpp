#include "GameScene.h"
#include "util.h"
#include "text.h"
#include "Enemy.h"

GameScene::GameScene()
{
	player = new Player();
	this->addGameObject(player);

	spawnTime = 300;
	currentSpawnTime = spawnTime;
}

GameScene::~GameScene()
{

}

void GameScene::start()
{
	Scene::start();
	initFonts();
	points = 0;

	backgroundTexture = loadTexture("gfx/background.png");

	explodeTexture = loadTexture("gfx/explosion.png"); 

	if (backgroundTexture == nullptr || explodeTexture == nullptr)
	{
		std::cout << "ERROR LOADING TEXTURES" << std::endl;
	}

	spawnEnemy(2);
}

void GameScene::draw()
{
	if (backgroundTexture != nullptr)
	{
		SDL_Rect dest = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(app.renderer, backgroundTexture, nullptr, nullptr);
	}

	Scene::draw();

	for (int i = 0; i < activeExplosions.size(); i++)
	{
		blit(explodeTexture, activeExplosions[i].x, activeExplosions[i].y);
	}

	drawText
	(
		110, 20,
		255, 255, 255,
		TEXT_CENTER,
		"POINTS: %03d", points
	);

	if (!player->getIsAlive())
	{
		drawText
		(
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			255, 255, 255,
			TEXT_CENTER,
			"GAME OVER"
		);
	}
}

void GameScene::update()
{
	Scene::update();
	doSpawnLogic();
	doCollisionsLogic();

	for (int i = activeExplosions.size() - 1; i >= 0; i--)
	{
		activeExplosions[i].timer -= 0.016f;
		if (activeExplosions[i].timer <= 0.0f)
		{
			activeExplosions.erase(activeExplosions.begin() + i);
		}
	}

	for (int i = enemies.size() - 1; i >= 0; i--)
	{
		if (enemies[i]->getX() < -enemies[i]->getWidth())
		{
			despawnEnemy(enemies[i], false);
		}
	}

	if (!player->getIsAlive() && app.keyboard[SDL_SCANCODE_R])
	{
		Scene::setActiveScene(new GameScene());
		return; 
	}
}

void GameScene::doSpawnLogic()
{
	if (!player->getIsAlive()) return;

	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}
	else
	{
		if (enemies.size() < 6) 
		{
			spawnEnemy(1);
		}
		currentSpawnTime = spawnTime;
	}

	std::cout << "enemies alive: " << enemies.size() << std::endl;
}

void GameScene::doCollisionsLogic()
{
	for (int i = 0; i < objects.size(); i++)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(objects[i]);
		if (bullet != NULL)
		{
			if (bullet->Getside() == Side::ENEMY_SIDE)
			{
				if (!player->getIsAlive())
				{
					continue;
				}

				int collision = checkCollision
				(
					bullet->getX(), bullet->getY(), bullet->getWidth(), bullet->getHeight(),
					player->getX(), player->getY(), player->getWidth(), player->getHeight()
				);

				if (collision == 1)
				{
					player->getDead();
					removeGameObject(bullet);
					delete bullet;
					return;
				}
			}
			else if (bullet->Getside() == Side::PLAYER_SIDE)
			{
				for (int j = 0; j < enemies.size(); j++)
				{
					Enemy* enemy = enemies[j];

					int collision = checkCollision
					(
						bullet->getX(), bullet->getY(), bullet->getWidth(), bullet->getHeight(),
						enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
					);

					if (collision == 1)
					{
						despawnEnemy(enemy);
						removeGameObject(bullet);
						delete bullet;
						points++;
						std::cout << "enemy" << std::endl;
						return;
					}
				}
			}
		}
	}
}

void GameScene::spawnEnemy(int count)
{
	int spacing = 60;
	int startY = (SCREEN_HEIGHT / 2) - ((count * spacing) / 2);

	for (int i = 0; i < count; i++)
	{
		int spawnX = SCREEN_WIDTH + 10;
		int spawnY = startY + (i * spacing);

		Enemy* enemy = new Enemy(player, spawnX, spawnY);
		this->addGameObject(enemy);
		enemies.push_back(enemy);
	}
}

void GameScene::despawnEnemy(Enemy* enemy, bool explode)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i] == enemy)
		{
			if (explode)
			{
				activeExplosions.emplace_back((float)enemy->getX(), (float)enemy->getY());
			}
			enemies.erase(enemies.begin() + i);
			removeGameObject(enemy);
			delete enemy;
			break;
		}
	}
}

void GameScene::cleanUp()
{
	for (int i = (int)enemies.size() - 1; i >= 0; i--)
	{
		if (enemies[i]->getX() < -enemies[i]->getWidth())
			despawnEnemy(enemies[i], false);
	}

	for (int i = (int)objects.size() - 1; i >= 0; i--)
	{
		Bullet* b = dynamic_cast<Bullet*>(objects[i]);
		if (b != NULL && (b->getX() < 0 || b->getX() > SCREEN_WIDTH ||
			b->getY() < 0 || b->getY() > SCREEN_HEIGHT))
		{
			removeGameObject(b);
			delete b;
		}
	}
}


