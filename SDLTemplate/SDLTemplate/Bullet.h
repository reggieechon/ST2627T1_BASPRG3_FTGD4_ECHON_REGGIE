#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

enum class Side
{
	PLAYER_SIDE,
	ENEMY_SIDE
};

class Bullet : public GameObject
{
public:
	Bullet(int positionX, int positionY, int directionX, int directionY, int speed, Side side);
	void start() override;
	void update() override;
	void draw() override;
	Side Getside();

private:
	SDL_Texture* texture;
	int speed;

	Side side;

	float directionX;
	float directionY;
};
