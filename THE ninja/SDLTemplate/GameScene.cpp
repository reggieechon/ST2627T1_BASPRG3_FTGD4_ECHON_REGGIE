#include "GameScene.h"
#include "player.h"
#include "platform.h"
#include "algorithm"

const int tileWidth = 64; 
const int margin = 250;                   
const int hugeL = margin;            
const int hugeR = SCREEN_WIDTH - margin;  

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
	SoundManager::loadMusic("music/Sekiro Shadows Die Twice OST.mp3");
	//music->volume = 24;

	int width = randTiles(2, 4);
	int x = randRange(hugeL, hugeR - width);
	int y = SCREEN_HEIGHT - 100;
	addGameObject(new platform(x, y, width, 10));

	int prevCenter = x + width / 2;

	while (true)
	{
		y -= randRange(80, 110);   
		if (y < 100) break;

		x = randRange(hugeL, hugeR - width);
		int center = prevCenter + randRange(-200, 200);
		x = center - width / 2;

		if (x < 0) 
		{ 
			x = 0; 
		}

		if (x > SCREEN_WIDTH - width)
		{
			x = SCREEN_WIDTH - width;
		}

		addGameObject(new platform(x, y, width, 10));
		prevCenter = x + width / 2;
	}
}

GameScene::~GameScene()
{
	
}

void GameScene::start()
{
	Scene::start();
	SoundManager::playMusic(1);
	backgroundTexture = loadTexture("assets/Layer_1.png");
	// Initialize any scene logic here
}

void GameScene::draw()
{
	Scene::draw();
}

void GameScene::update()
{
	Scene::update();
}
