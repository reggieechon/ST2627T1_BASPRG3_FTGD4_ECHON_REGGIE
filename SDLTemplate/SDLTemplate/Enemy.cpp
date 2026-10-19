#include "Enemy.h"
#include "GameScene.h"
#include "GameObject.h"


Enemy::Enemy(Player* player, int xPos, int yPos)
{
	targetPlayer = player;
	x = xPos;
	y = yPos;
}

Enemy::~Enemy()
{

}

void Enemy::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/enemy.png");
	sound = SoundManager::loadSound("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");
	sound->volume = 25;

	// Initialize to avoid garbage values
	width = 1000;
	height = 300;
	speed = 2;

	directionX = 1;
	directionY = 1;
	directionChangeTime = (rand() % 300) + 180;
	currentDirectionChangeTime = 0;

	reloadTime = 60;
	currentReloadTime = 0;
	

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Enemy::update()
{
	if (currentDirectionChangeTime > 0)
	{
		currentDirectionChangeTime--;
	}
	else
	{
		directionX = -directionX;     
		currentDirectionChangeTime = directionChangeTime;
	}

	if (x < 0)
	{
		x = 0;
		directionX = 1;
	}
	else if (x > SCREEN_WIDTH - width)
	{
		x = SCREEN_WIDTH - width;
		directionX = -1;
	}

	x += directionX * speed;
	y += directionY * speed;

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}
	else if (y > 0)
	{
		float bulletDirectionX;
		float bulletDirectionY;

		calcSlope
		(
			targetPlayer->getX(), targetPlayer->getY(),
			x + (width / 2), y + height,
			&bulletDirectionX, &bulletDirectionY
		);

		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet
		(
			x + (width / 2),
			y + height,
			bulletDirectionX,
			bulletDirectionY,
			5,
			Side::ENEMY_SIDE
		);

		getScene()->addGameObject(bullet);

		currentReloadTime = reloadTime;
	}

}

void Enemy::setPosition(int xPos, int yPos)
{
	this->x = xPos;
	this->y = yPos;
}

void Enemy::draw()
{
	blit(texture, x, y);
}