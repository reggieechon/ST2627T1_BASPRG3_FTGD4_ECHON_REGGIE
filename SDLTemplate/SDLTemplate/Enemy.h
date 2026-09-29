#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include "player.h"
#include "util.h"
#include <vector>

class Enemy:
	public GameObject
{
public:
	Enemy(Player* player);
	~Enemy();
	void start() override;
	void update() override;
	void draw() override;

private:

	SDL_Texture* texture;
	Mix_Chunk* sound;

	int speed;
	int directionX;
	int directionY;
	float diretionChangeTime;
	float currentDirectionChaneTime;

	float reloadTime;
	float currentReloadTime;

	std::vector<Bullet*> bullets;
	Player* targetPlayer;
};

