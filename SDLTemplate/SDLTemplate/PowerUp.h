#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

enum class PowerUpType
{
	SPEED,
	DOUBLE_SHOT,
	BURST
};

class PowerUp : public GameObject
{
public:

	SDL_Texture* texture;

	PowerUp(PowerUpType type, int xPos, int yPos);
	void start() override;
	void update() override;
	void draw() override;
	PowerUpType getType();

private:
	PowerUpType type;
	float speed;
};