#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "player.h"
#include "Enemy.h"
#include "text.h"
#include "PowerUp.h"
#include "bossAI.h"
#include <vector> 

struct Explosion 
{
    float x, y;
    float timer;

    Explosion(float posX, float posY) 
    {
        x = posX;
        y = posY;
        timer = 0.5f; 
    }
};
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
    void despawnEnemy(Enemy* enemy, bool explode = true);
    void cleanUp();
    void spawnPowerUp();
    void doPowerUpLogic();

    void spawnBoss();
    void doBossLogic();


    Player* player;
    Enemy* enemy;
    bossAI* boss;
    int points;
    int bossesSpawned;

    float powerUpSpawnTime;
    float spawnTime;
    float currentSpawnTime;

    SDL_Texture* backgroundTexture;
    SDL_Texture* explodeTexture;

    std::vector<Enemy*> enemies;

    std::vector<Explosion> activeExplosions;
};