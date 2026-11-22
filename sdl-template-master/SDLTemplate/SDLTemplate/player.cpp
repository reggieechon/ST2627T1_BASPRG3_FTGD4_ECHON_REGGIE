#include "Player.h"
#include "GameScene.h"
#include "GameObject.h"
#include "platform.h"
#include <cmath>

player::~player()
{

}

void player::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("assets/player.png");
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
	velocityY = 0;
	onGround = false;
}

void player::update()
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
			getScene()->addGameObject(new projectile(x + 5, y, 0, -1, 5, Side::PLAYER_SIDE));
			getScene()->addGameObject(new projectile(x + width - 15, y, 0, -1, 5, Side::PLAYER_SIDE));
		}
		else
		{
			getScene()->addGameObject(new projectile(x + (width / 2) - 5, y, 0, -1, 5, Side::PLAYER_SIDE));
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

			getScene()->addGameObject(new projectile(x + (width / 2) - 5, y, dirX, dirY, 5, Side::PLAYER_SIDE));
		}

		burstCooldown = 120;
	}

	const int GRAVITY = 1;
	const int JUMP_STRENGTH = 16;
	const int MAX_FALL_SPEED = 15;

	// ---- Horizontal movement, then resolve ----
	int dx = 0;
	if (app.keyboard[SDL_SCANCODE_A]) dx -= currentSpeed;
	if (app.keyboard[SDL_SCANCODE_D]) dx += currentSpeed;
	x += dx;

	for (GameObject* obj : getScene()->objects())
	{
		platform* p = dynamic_cast<platform*>(obj);
		if (p == nullptr || !overlap(p)) continue;

		if (dx > 0)      x = p->getX() - width;            // hit its left side
		else if (dx < 0) x = p->getX() + p->getWidth();    // hit its right side
	}

	// ---- Jump ----
	if (app.keyboard[SDL_SCANCODE_SPACE] && onGround)
	{
		velocityY = -JUMP_STRENGTH;
	}

	// ---- Gravity, then resolve ----
	velocityY += GRAVITY;
	if (velocityY > MAX_FALL_SPEED) velocityY = MAX_FALL_SPEED;
	y += velocityY;
	onGround = false;

	for (GameObject* obj : getScene()->getObjects())
	{
		platform* p = dynamic_cast<platform*>(obj);
		if (p == nullptr || !overlap(p)) continue;

		if (velocityY > 0)        // falling: land on top
		{
			y = p->getY() - height;
			velocityY = 0;
			onGround = true;
		}
		else if (velocityY < 0)   // rising: bonk head
		{
			y = p->getY() + p->getHeight();
			velocityY = 0;
		}
	}

	// ---- Screen bounds ----
	if (x < 0) x = 0;
	if (x > SCREEN_WIDTH - width) x = SCREEN_WIDTH - width;
	if (y < 0) y = 0;
	if (y > SCREEN_HEIGHT - height)
	{
		y = SCREEN_HEIGHT - height;
		velocityY = 0;
		onGround = true;   // screen bottom acts as floor
	}
}

void player::draw()
{
	if (isAlive)
	{
		blit(texture, x, y);
	}
}

bool player::getIsAlive()
{
	return isAlive;
}

void player::getDead()
{
	isAlive = false;
}

//void Player::getPowerUp(PowerUpType type)
//{
//	if (type == PowerUpType::SPEED)
//	{
//		speedBoostTimer = 600;
//	}
//	else if (type == PowerUpType::DOUBLE_SHOT)
//	{
//		doubleShotTimer = 900;
//	}
//	else if (type == PowerUpType::BURST)
//	{
//		burstTimer = 900;
//	}
//}
