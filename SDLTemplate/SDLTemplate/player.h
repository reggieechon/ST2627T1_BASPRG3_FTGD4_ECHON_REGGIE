#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include "PowerUp.h"
#include <vector>

class Player : public GameObject
{
public:
	~Player();
	void start() override;
	void update() override;
	void draw() override;
	bool getIsAlive();
	void getDead();

	void getPowerUp(PowerUpType type);

private:
	SDL_Texture* texture;
	int currentSpeed;

	Mix_Chunk* sound;

	bool isAlive;

	int defaultSpeed;
	int boostedSpeed;

	int speedBoostTimer;
	int doubleShotTimer;
	int burstTimer;
	int burstCooldown;

	float reloadTime;
	float altreloadTime;
	float currentReloadTime;
	float altcurrentReloadTime;
};