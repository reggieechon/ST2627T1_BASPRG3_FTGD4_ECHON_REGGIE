#include "GameScene.h"
#include "util.h"
#include "text.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
	player = new Player();
	this->addGameObject(player);
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

	spawnTime = 120;
	currentSpawnTime = spawnTime;

	spawnEnemy(3);
	// Initialize any scene logic here
}

void GameScene::draw()
{
	Scene::draw();

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
					bullet->getX(),
					bullet->getY(),
					bullet->getWidth(),
					bullet->getHeight(),

					player->getX(),
					player->getY(),
					player->getWidth(),
					player->getHeight()
				);

				if (collision == 1)
				{
					std::cout << "Player hit" << std::endl;

					break;
				}
			}
			else if (bullet->Getside() == Side::PLAYER_SIDE)
			{
				for (int i = 0; i < enemies.size(); i++)
				{
					Enemy* enemy = enemies[i];

					int collision = checkCollision
					(
						bullet->getX(),
						bullet->getY(),
						bullet->getWidth(),
						bullet->getHeight(),

						enemy->getX(),
						enemy->getY(),
						enemy->getWidth(),
						enemy->getHeight()
					);

					if (collision == 1)
					{
						despawnEnemy(enemy);
						points + 1;
						std::cout << "eney" << std::endl;

						break;
					}
				}
			}
		}
	}
}

void GameScene::spawnEnemy(int count)
{
	for (int i = 0; i < count; i++)
	{
		Enemy* enemy = new Enemy(player);
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
		enemies.erase(enemies.begin() + index);
		delete enemy;
	}
}
