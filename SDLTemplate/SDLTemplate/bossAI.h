#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "Bullet.h"
#include "player.h"

class bossAI : public GameObject
{
public:
	bossAI(Player* player, int tier, int xPos, int yPos);
	void start() override;
	void update() override;
	void draw() override;

	bool takeDamage(int amount);  

private:
	void fireAimed(int count, float spread);
	void fireRadial(int count);

	Player* player;
	SDL_Texture* texture;

	int tier;          
	int health;
	int maxHealth;

	float moveDir;
	float moveSpeed;
	int targetY;
	bool entering;

	int aimedTimer;
	int radialTimer;
};