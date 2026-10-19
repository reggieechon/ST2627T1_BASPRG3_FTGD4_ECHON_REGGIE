#include "Player.h"
#include "GameScene.h"
#include "GameObject.h"
#include <cmath>

Player::~Player()
{

}

void Player::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/player.png");
	sound = SoundManager::loadSound("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");
	sound->volume = 24;

	// Initialize to avoid garbage values
	width = 0;
	height = 0;

	isAlive = true;

	defaultSpeed = 5;
	boostedSpeed = 8;
	currentSpeed = defaultSpeed;

	reloadTime = 8; // 0.16sec (8/60)
	altreloadTime = 16;
	currentReloadTime = 0;
	altcurrentReloadTime = 0;

	speedBoostTimer = 0;
	doubleShotTimer = 0;
	burstTimer = 0;
	burstCooldown = 0;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	x = (SCREEN_WIDTH / 2) - (width / 2);
	y = SCREEN_HEIGHT - height - 50;
}

void Player::update()
{

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}

	if (altcurrentReloadTime > 0)
	{
		altcurrentReloadTime--;
	}

	if (speedBoostTimer > 0) 
	{
		speedBoostTimer--;
	}

	if (doubleShotTimer > 0)  
	{
		doubleShotTimer--;
	}

	if (burstTimer > 0)      
	{
		burstTimer--;
	}

	if (burstCooldown > 0)    
	{
		burstCooldown--;
	}

	currentSpeed = (speedBoostTimer > 0) ? boostedSpeed : defaultSpeed;

	if (!isAlive)
	{
		return;
	}

	if (app.keyboard[SDL_SCANCODE_F] && currentReloadTime <= 0)
	{
		SoundManager::playSound(sound);

		if (doubleShotTimer > 0)
		{
			getScene()->addGameObject(new Bullet(x + 5, y, 0, -1, 5, Side::PLAYER_SIDE));
			getScene()->addGameObject(new Bullet(x + width - 15, y, 0, -1, 5, Side::PLAYER_SIDE));
		}
		else
		{
			getScene()->addGameObject(new Bullet(x + (width / 2) - 5, y, 0, -1, 5, Side::PLAYER_SIDE));
		}

		currentReloadTime = reloadTime;
	}

	if (app.keyboard[SDL_SCANCODE_E] && burstTimer > 0 && burstCooldown <= 0)
	{
		SoundManager::playSound(sound);

		for (int i = 0; i < 8; i++)
		{
			float angle = (-70 + i * 20) * 3.14159f / 180.0f;  
			float dirX = sinf(angle);
			float dirY = -cosf(angle);

			getScene()->addGameObject(new Bullet(x + (width / 2) - 5, y, dirX, dirY, 5, Side::PLAYER_SIDE));
		}

		burstCooldown = 120;
	}

	if (app.keyboard[SDL_SCANCODE_W])
	{
		y -= currentSpeed;
	}
	if (app.keyboard[SDL_SCANCODE_S])
	{
		y += currentSpeed;
	}
	if (app.keyboard[SDL_SCANCODE_A])
	{
		x -= currentSpeed;
	}
	if (app.keyboard[SDL_SCANCODE_D])
	{
		x += currentSpeed;
	}

	if (x < 0) x = 0;

	if (x > SCREEN_WIDTH - width) x = SCREEN_WIDTH - width;

	if (y < 0) y = 0;

	if (y > SCREEN_HEIGHT - height) y = SCREEN_HEIGHT - height;
}

void Player::draw()
{
	if(isAlive)
	{
		blit(texture, x, y);
	}
}

bool Player::getIsAlive()
{
	return isAlive;
}

void Player::getDead()
{
	isAlive = false;
}

void Player::getPowerUp(PowerUpType type)
{
	if (type == PowerUpType::SPEED)
	{
		speedBoostTimer = 600;
	}  
	else if (type == PowerUpType::DOUBLE_SHOT)
	{
		doubleShotTimer = 900;
	}
	else if (type == PowerUpType::BURST)
	{
		burstTimer = 900;
	}
}
