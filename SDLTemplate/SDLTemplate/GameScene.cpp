#include "GameScene.h"
#include "util.h"
#include "text.h"
#include "Enemy.h"

GameScene::GameScene()
{
	player = new Player();
	this->addGameObject(player);
	SoundManager::loadMusic("music/BackgroundMusic.mp3");

	spawnTime = 120;
	currentSpawnTime = spawnTime;

	powerUpSpawnTime = 480;

	boss = nullptr;
	bossesSpawned = 0;
	powerUpSpawnTime = 300;
}

GameScene::~GameScene()
{

}

void GameScene::start()
{
	SoundManager::playMusic(1);
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
	cleanUp();
	doPowerUpLogic();
	doBossLogic();

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
	powerUpSpawnTime--;
	if (powerUpSpawnTime <= 0)
	{
		spawnPowerUp();
		powerUpSpawnTime = 480 + (rand() % 300);  
	}

	if (boss != nullptr)
	{
		return;
	}

	if (!player->getIsAlive()) return;

	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}
	else
	{
		if (enemies.size() < 6) 
		{
			spawnEnemy(2);
		}
		if (spawnTime > 30)
		{
			spawnTime -= 2;
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
				if (boss != nullptr)
				{
					int hit = checkCollision
					(
						bullet->getX(), bullet->getY(), bullet->getWidth(), bullet->getHeight(),
						boss->getX(), boss->getY(), boss->getWidth(), boss->getHeight()
					);

					if (hit == 1)
					{
						removeGameObject(bullet);
						delete bullet;

						if (boss->takeDamage(1))
						{
							activeExplosions.emplace_back((float)boss->getX(), (float)boss->getY());
							removeGameObject(boss);
							delete boss;
							boss = nullptr;
						}
						return;
					}
				}

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
		int spawnX = 50 + (rand() % (SCREEN_WIDTH - 150));
		int spawnY = -100 - (rand() % 200);   

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
		if (enemies[i]->getY() > SCREEN_HEIGHT)
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

void GameScene::spawnPowerUp()
{
	PowerUpType type = (PowerUpType)(rand() % 3);
	int spawnX = 30 + (rand() % (SCREEN_WIDTH - 60));

	PowerUp* pUp = new PowerUp(type, spawnX, -40);
	this->addGameObject(pUp);
}

void GameScene::doPowerUpLogic()
{
	for (int i = (int)objects.size() - 1; i >= 0; i--)
	{
		PowerUp* pUp = dynamic_cast<PowerUp*>(objects[i]);
		if (pUp == NULL) continue;

		int collision = checkCollision
		(
			pUp->getX(), pUp->getY(), pUp->getWidth(), pUp->getHeight(),
			player->getX(), player->getY(), player->getWidth(), player->getHeight()
		);

		if (collision == 1 && player->getIsAlive())
		{
			player->getPowerUp(pUp->getType());
			removeGameObject(pUp);
			delete pUp;
		}
		else if (pUp->getY() > SCREEN_HEIGHT)
		{
			removeGameObject(pUp);
			delete pUp;
		}
	}
}

void GameScene::doBossLogic()
{
	if (boss == nullptr && bossesSpawned < 3 && points >= 10 * (bossesSpawned + 1))
	{
		spawnBoss();
	}
}

void GameScene::spawnBoss()
{
	bossesSpawned++;
	boss = new bossAI(player, bossesSpawned, (SCREEN_WIDTH / 2) - 64, -150);
	this->addGameObject(boss);
}


