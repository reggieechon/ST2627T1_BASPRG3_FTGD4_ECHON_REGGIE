#include "GameScene.h"
#include "player.h"
#include "platform.h"
#include "algorithm"
#include "text.h"
#include <fstream>

using namespace std;

const int tileWidth = 64; 
const int margin = 250;                   
const int hugeL = margin;            
const int hugeR = SCREEN_WIDTH - margin;

static const char* hsFile = "Current Highscore.txt";

static int loadHS()
{
	ifstream in(hsFile);

	int scor = 0;
	if (in >> scor)
	{
		return scor;
	}

	return 0;
}

static void saveHS(int sco)
{
	ofstream out(hsFile);
	out << sco;
}

static int randRange(int low, int high)
{
	return low + rand() % (high - low + 1);
}

static int randTiles(int minTiles, int maxTiles)
{
	return (minTiles + rand() % (maxTiles - minTiles + 1)) * tileWidth;
}

GameScene::GameScene()
{
	player = new Player();
	this->addGameObject(player);
	SoundManager::loadMusic("music/bgm 1.mp3");
	
	/*music2 = SoundManager::loadSound("music/bgm 2.mp3");
	music2->volume = 24;*/
	
	int platformHeight = 64;
	int width = randTiles(1, 2);
	int x = randRange(hugeL, hugeR - width);
	int y = SCREEN_HEIGHT - 100;
	addGameObject(new platform(x, y, width, platformHeight));

	int prevCenter = x + width / 2;

	while (true)
	{
		y -= randRange(140, 190);   
		if (y < 140) break;

		x = randRange(hugeL, hugeR - width);
		int center = prevCenter + randRange(-100, 100);
		x = center - width / 2;

		if (x < 0) 
		{ 
			x = 0; 
		}

		if (x > SCREEN_WIDTH - width)
		{
			x = SCREEN_WIDTH - width;
		}

		addGameObject(new platform(x, y, width, platformHeight));
		prevCenter = x + width / 2;
	}
}

GameScene::~GameScene()
{
	
}

void GameScene::start()
{
	SoundManager::playMusic(1);
	SoundManager::setMusicVolume(24);
	Scene::start();
	initFonts();
	points = 0;

	highScore = loadHS();
	savedScore = false;
	newHS = false;

	backgroundTexture = loadTexture("assets/terraria.png");
	SDL_ShowCursor(1);
	// Initialize any scene logic here
}

static void drawDim(int alpha)
{
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, alpha);

	SDL_Rect full = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(app.renderer, &full);

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

void GameScene::draw()
{
	if (backgroundTexture != nullptr)
	{
		SDL_Rect dest = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(app.renderer, backgroundTexture, nullptr, nullptr);
	}

	Scene::draw();

	drawText
	(
		110, 20,
		255, 255, 255,
		TEXT_CENTER,
		"POINTS: %03d", points
	);
	

	drawText
	(
		580, 20,
		255, 255, 255,
		TEXT_CENTER,
		"BEST SCORE: %03d", highScore
	);

	drawText
	(
		20, 910,
		255, 255, 255,
		TEXT_LEFT,
		"PRESS F TO TELEPORT"
	);

	if (!player->getIsAlive())
	{
		drawDim(150);

		drawText
		(
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			255, 255, 255,
			TEXT_CENTER,
			"GAME OVER"
		);

		drawText
		(
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 40,
			255, 255, 255,
			TEXT_CENTER,
			"R TO RESTART"
		);

		if (newHS)
		{
			drawText
			(
				SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40, 
				255, 255, 0, 
				TEXT_CENTER,
				"NEW HIGH SCORE"
			);
		}
	}
}

void GameScene::update()
{
	Scene::update();
	doSpawnLogic();
	doCollisionsLogic();
	cleanUp(); 
	doBossLogic();

	if(!player->getIsAlive() && !savedScore)
	{
		savedScore = true;

		if (points > highScore)
		{
			highScore = points;
			newHS = true;
			saveHS(highScore);
		}
	}

	if (app.keyboard[SDL_SCANCODE_R])
	{
		Scene::setActiveScene(new GameScene());
		return;
	}
}

void GameScene::doSpawnLogic()
{
	//powerUpSpawnTime--;
	//if (powerUpSpawnTime <= 0)
	//{
	//	spawnPowerUp();
	//	powerUpSpawnTime = 480 + (rand() % 300);
	//}

	if (boss != nullptr)
	{
		backgroundTexture = loadTexture("assets/terraria hell.png");
		return;
	}
	else
	{
		backgroundTexture = loadTexture("assets/terraria.png");
	}

	if (!player->getIsAlive()) return;

	if (currentSpawnTime > 0)
	{
		currentSpawnTime--;
	}
	else
	{
		if (enemies.size() < 2)
		{
			spawnEnemy(1);
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
		projectile* proj = dynamic_cast<projectile*>(objects[i]);
		if (proj != NULL)
		{
			if (proj->Getside() == Side::ENEMY_SIDE)
			{
				if (!player->getIsAlive())
				{
					continue;
				}

				SDL_Rect playerHB = player->getHitBox();
				int collision = checkCollision
				(
					proj->getX(), proj->getY(), proj->getWidth(), proj->getHeight(),
					playerHB.x, playerHB.y, playerHB.w, playerHB.h
				);

				if (collision == 1)
				{
					removeGameObject(proj);
					delete proj;

					if (player->takeDamage(1))
					{
						player->getDead();
					}
					return;
				}
			}
			else if (proj != nullptr && proj->Getside() == Side::PLAYER_SIDE)
			{
				if (boss != nullptr)
				{
					int hit = checkCollision
					(
						proj->getX(), proj->getY(), proj->getWidth(), proj->getHeight(),
						boss->getX(), boss->getY(), boss->getWidth(), boss->getHeight()
					);

					if (hit == 1)
					{
						removeGameObject(proj);
						delete proj;

						if (boss->takeDamage(1))
						{
							//activeExplosions.emplace_back((float)boss->getX(), (float)/boss-/>getY());
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
						proj->getX(), proj->getY(), proj->getWidth(), proj->getHeight(),
						enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
					);

					if (collision == 1)
					{
						despawnEnemy(enemy);
						removeGameObject(proj);
						delete proj;
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
	int startX = (SCREEN_WIDTH / 2) - ((count * spacing) / 2);

	for (int i = 0; i < count; i++)
	{
		bool leftSpawn = (rand() % 2) == 0;
		int spawnX = leftSpawn ? 0 : (SCREEN_WIDTH - 64);
		int spawnY = 50 + (rand() % (SCREEN_HEIGHT - 150));

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
			/*if (explode)
			{
				activeExplosions.emplace_back((float)enemy->getX(), (float)enemy->getY());
			}*/
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
		projectile* b = dynamic_cast<projectile*>(objects[i]);
		if (b != NULL && (b->getX() < 0 || b->getX() > SCREEN_WIDTH ||
			b->getY() < 0 || b->getY() > SCREEN_HEIGHT))
		{
			removeGameObject(b);
			delete b;
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
	boss = new Boss(player, bossesSpawned, (SCREEN_WIDTH / 2) - 64, -150);
	this->addGameObject(boss);
}
