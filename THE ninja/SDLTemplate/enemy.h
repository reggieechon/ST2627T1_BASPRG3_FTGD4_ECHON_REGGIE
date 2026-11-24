#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "projectile.h"
#include "player.h"
#include "util.h"
#include <vector>

class Enemy :
	public GameObject
{
public:
	Enemy(Player* player, int xPos, int yPos);
	~Enemy();
	void start() override;
	void update() override;
	void draw() override;
	void setPosition(int xPos, int yPos);
	bool takeDamage(int amount);

private:

	SDL_Texture* texture;
	Mix_Chunk* sound;
	Player* targetPlayer;

	int speed;
	float directionX;
	float directionY;
	float directionChangeTime;
	float currentDirectionChangeTime;

	int tier;
	int health;
	int maxHealth;

	float reloadTime;
	float currentReloadTime;
};

