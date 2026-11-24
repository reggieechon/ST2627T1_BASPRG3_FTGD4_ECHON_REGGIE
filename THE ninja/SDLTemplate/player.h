#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "projectile.h"
#include <vector>

class Player : public GameObject
{
public:
	~Player();
	void start() override;
	SDL_Rect getHitBox();
	void update() override;
	void draw() override;
	bool getIsAlive();
	void getDead();
	bool takeDamage(int amount);


private:
	SDL_Texture* texture;
	int currentSpeed;

	Mix_Chunk* sound;
	Mix_Chunk* jump;
	Mix_Chunk* grunt;

	bool isAlive;
	int velocityY;
	bool onGround;

	int defaultSpeed;
	int boostedSpeed;

	int speedBoostTimer;
	int doubleShotTimer;
	int burstTimer;
	int burstCooldown;
	int tier;
	int health;
	int maxHealth;
	int tpCooldown = 0;

	float reloadTime;
	float altreloadTime;
	float currentReloadTime;
	float altcurrentReloadTime;
};