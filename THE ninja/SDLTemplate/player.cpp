#include "player.h"
#include "GameScene.h"
#include "GameObject.h"
#include "platform.h"
#include "Scene.h"
#include <cmath>

using namespace std;

Player::~Player()
{

}

void Player::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("assets/player.png");

	sound = SoundManager::loadSound("sfx/knife2.mp3");
	sound->volume = 24;

	jump = SoundManager::loadSound("sfx/jump.mp3");
	jump->volume = 18;

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

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	x = (SCREEN_WIDTH / 2) - (width / 2);
	y = SCREEN_HEIGHT - height - 50;
	velocityY = 0;
	onGround = false;
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

	if (app.mouseButtons[SDL_BUTTON_LEFT] && currentReloadTime <= 0)
	{
		SoundManager::playSound(sound);

		int spawnX = x + (width / 2) - 5;
		int spawnY = y;

		float delX = static_cast<float>(app.mouseX - spawnX);
		float delY = static_cast<float>(app.mouseY - spawnY);
		float length = sqrt(delX * delX + delY * delY);

		float dirX = 0.0f;
		float dirY = -1.0f;
		if (length > 0.0001f)
		{
			dirX = delX / length;
			dirY = delY / length;
		}

		getScene()->addGameObject(new projectile(spawnX, spawnY, dirX, dirY, 10, Side::PLAYER_SIDE));
		cout << "shing" << endl;

		currentReloadTime = reloadTime;
	}

	//if (app.keyboard[SDL_SCANCODE_E] && burstTimer > 0 && burstCooldown <= 0)
	//{
	//	SoundManager::playSound(sound);

	//	for (int i = 0; i < 8; i++)
	//	{
	//		float angle = (-70 + i * 20) * 3.14159f / 180.0f;
	//		float dirX = sinf(angle);
	//		float dirY = -cosf(angle);

	//		getScene()->addGameObject(new projectile(x + (width / 2) - 5, y, dirX, dirY, 5, Side::PLAYER_SIDE));
	//	}

	//	burstCooldown = 120;
	//}

	const int gravity = 1;
	const int jumpStrength = 16;
	const int fallSpeed = 15;

	int dx = 0;

	if (app.keyboard[SDL_SCANCODE_A]) 
	{
		dx -= currentSpeed;
	}

	if (app.keyboard[SDL_SCANCODE_D]) 
	{
		dx += currentSpeed;
	}

	x += dx;

	for (GameObject* obj : getScene()->getObjects())
	{
		platform* p = dynamic_cast<platform*>(obj);
		if (p == nullptr || !overlaping(p)) continue;

		if (dx > 0)     
		{
			x = p->getX() - width;           
		}
		else if (dx < 0) 
		{
			x = p->getX() + p->getWidth();  
		}
	}

	// ---- Jump ----
	if (app.keyboard[SDL_SCANCODE_SPACE] && onGround)
	{
		SoundManager::playSound(jump);
		velocityY = -jumpStrength;
	}

	// ---- Gravity, then resolve ----
	velocityY += gravity;
	if (velocityY > fallSpeed) velocityY = fallSpeed;
	y += velocityY;
	onGround = false;

	for (GameObject* obj : getScene()->getObjects())
	{
		platform* p = dynamic_cast<platform*>(obj);
		if (p == nullptr || !overlaping(p)) continue;

		if (velocityY > 0)    
		{
			y = p->getY() - height;
			velocityY = 0;
			onGround = true;
		}
		else if (velocityY < 0) 
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
		onGround = true;  
	}
}

void Player::draw()
{
	if (isAlive)
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
