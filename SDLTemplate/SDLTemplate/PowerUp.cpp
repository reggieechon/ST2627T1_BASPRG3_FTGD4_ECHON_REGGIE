#include "PowerUp.h"
#include "GameScene.h"
#include "GameObject.h"

PowerUp::PowerUp(PowerUpType type, int xPos, int yPos)
{
	this->type = type;
	x = xPos;
	y = yPos;
}

void PowerUp::start()
{
	width = 30;
	height = 30;
	speed = 2;
}

void PowerUp::update()
{
	y += speed;  
}

void PowerUp::draw()
{
	SDL_Rect rect = { (int)x, (int)y, width, height };

	if (type == PowerUpType::SPEED)            
	{
		texture = loadTexture("gfx/SU_Powerup.png");
		blit(texture, x, y);
	}

	else if (type == PowerUpType::DOUBLE_SHOT) 
	{
		texture = loadTexture("gfx/DB_Powerup.png"); 
		blit(texture, x, y);
	}

	else                                      
	{
		texture = loadTexture("gfx/8S_Powerup.png"); 
		blit(texture, x, y);
	}
}

PowerUpType PowerUp::getType()
{
	return type;
}