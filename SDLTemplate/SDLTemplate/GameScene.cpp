#include "GameScene.h"
#include "util.h"
#include "text.h"
#include "Enemy.h"

GameScene::GameScene()
{
	player = new Player();
	this->addGameObject(player);

	enemy = new Enemy();
	this->addGameObject(enemy);

	spawnTime = 300;
	currentSpawnTime = spawnTime;

	for (int i = 0; i < 3; i++)
	{
		Enemy* enemy = new Enemy();
		this->addGameObject(enemy);
		enemy->targetPlayer(player);

		enemy->setPosition(1200, 300 + (rand() % 300));
		spawnedEnemies.push_back(enemy);
	}
}

GameScene::~GameScene()
{
	delete player;
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
}

void GameScene::doSpawnLogic()
{
	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}
	else
	{
		spawnEnemy(2);
		currentSpawnTime = spawnTime;
	}
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
				int collision = checkCollision
				(
					bullet->getX(), bullet->getY(), bullet->getWidth(), bullet->getHeight(),
					player->getX(), player->getY(), player->getWidth(), player->getHeight()
				);

				if (collision == 1)
				{
					std::cout << "Player hit" << std::endl;
					break;
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
						points++;
						std::cout << "enemy" << std::endl;
						break;
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

void GameScene::despawnEnemy(Enemy* enemy)
{
	int index = -1;

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemy == enemies[i])
		{
			index = i;
			break;
		}
	}

	if (index >= 0)
	{
		activeExplosions.push_back({ enemy->getX(), enemy->getY() });

		enemies.erase(enemies.begin() + index);
		delete enemy;
	}
}