#include "Enemy.h"
#include "GameScene.h"
#include "GameObject.h"


Enemy::Enemy(Player* player)
{
	targetPlayer = player;
	x = SCREEN_WIDTH + (rand() % 200);
	y = rand() % SCREEN_HEIGHT;
}

Enemy::~Enemy()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}

	bullets.clear();
}

void Enemy::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/enemy.png");
	sound = SoundManager::loadSound("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");
	sound->volume = 64;

	// Initialize to avoid garbage values
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	directionX = -1;
	directionY = 1;
	diretionChangeTime = 120;
	currentDirectionChaneTime = diretionChangeTime;

	reloadTime = 60;
	currentReloadTime = 0;
	

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Enemy::update()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getX() < SCREEN_WIDTH >  0)
		{
			Bullet* bulletToDelete = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToDelete;

			break;
		}
	}

	if (currentDirectionChaneTime > 0)
	{
		currentDirectionChaneTime--;
	}
	else
	{
		directionY = -directionY;

		currentDirectionChaneTime = diretionChangeTime;
	}

	if (y < 0 || y > SCREEN_HEIGHT - height)
	{
		directionY = -directionY;
	}

	x += directionX * speed;
	y += directionY * speed;

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}
	else
	{
		float bulletDirectionX;
		float bulletDirectionY;

		calcSlope
		(
			targetPlayer->getX(), targetPlayer->getY(), x, y, &bulletDirectionX, &bulletDirectionY
		);

		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet
		(
			x, 
			y + (height / 2) - 5,
			bulletDirectionX,
			bulletDirectionY,
			5,
			Side::ENEMY_SIDE
		);

		getScene()->addGameObject(bullet);
		bullets.push_back(bullet);

		currentReloadTime = reloadTime;
	}

}

void Enemy::draw()
{
	blit(texture, x, y);
}