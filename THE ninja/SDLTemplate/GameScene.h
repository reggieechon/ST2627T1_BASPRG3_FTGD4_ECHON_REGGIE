#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	Scene startScene;
	void start();
	void draw();
	void update();
private:

	void doSpawnLogic();
	void doCollisionsLogic();

	void spawnEnemy(int count);
	void despawnEnemy(Enemy* enemy, bool explode = true);
	void cleanUp();
	void doBossLogic();
	void spawnBoss();

	int points;
	int highScore = 0;
	int bossesSpawned;

	float spawnTime;
	float currentSpawnTime;

	bool savedScore = false;
	bool newHS = false;

	Player* player;
	Enemy* enemy;
	Boss* boss;

	SDL_Texture* backgroundTexture;
	Mix_Chunk* music;
	Mix_Chunk* music2;

	std::vector<Enemy*> enemies;
};

