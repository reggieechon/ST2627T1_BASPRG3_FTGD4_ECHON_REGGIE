#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "projectile.h"
#include <vector>

class player : public GameObject
{
public:
	~player();
	void start() override;
	void update() override;
	void draw() override;
	bool getIsAlive();
	void getDead();

private:
	SDL_Texture* texture;
	int currentSpeed;

	Mix_Chunk* sound;

	bool isAlive;
	int velocityY;
	bool onGround;

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