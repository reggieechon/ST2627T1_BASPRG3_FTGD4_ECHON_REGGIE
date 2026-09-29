#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "player.h"
#include "Enemy.h"
#include "text.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void start();
	void draw();
	void update();
private:
	void doSpawnLogic();
	void doCollisionsLogic();

	void spawnEnemy(int count);
	void despawnEnemy(Enemy* enemy);

	Player* player;
	int points;

	float spawnTime;
	float currentSpawnTime;

	std::vector<Enemy*> enemies;
};

