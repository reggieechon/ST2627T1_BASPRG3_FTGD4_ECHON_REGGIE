#include "enemy.h"
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
	texture = loadTexture("assets/enemy.png");
	sound = SoundManager::loadSound("sfx/enemy projectile sfx.mp3");
	sound->volume = 25;

	// Initialize to avoid garbage values
	width = 0;
	height = 0;
	speed = 2;

	directionX = 1;
	directionY = 0;
	directionChangeTime = (rand() % 300) + 180;
	currentDirectionChangeTime = 0;

	reloadTime = 120;
	currentReloadTime = 0;

	int hp[] = { 2, 3, 5, 10 };
	maxHealth = hp[tier];
	health = maxHealth;


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
			x + width, y + (height / 2),
			&bulletDirectionX, &bulletDirectionY
		);

		if (targetPlayer == nullptr || !targetPlayer->getIsAlive())
		{
			return;
		}

		SoundManager::playSound(sound);
		projectile* proj = new projectile
		(
			x + (width / 2),
			y + height,
			bulletDirectionX,
			bulletDirectionY,
			5,
			Side::ENEMY_SIDE
		);

		getScene()->addGameObject(proj);

		currentReloadTime = reloadTime;
	}
}

void Enemy::setPosition(int xPos, int yPos)
{
	this->x = xPos;
	this->y = yPos;
}

bool Enemy::takeDamage(int amount)
{
	health -= amount;
	return health <= 0;
}

void Enemy::draw()
{
	blit(texture, x, y);

	int barWidth = width;
	int filled = (int)((float)health / maxHealth * barWidth);

	SDL_Rect back = { (int)x, (int)y - 12, barWidth, 6 };
	SDL_Rect front = { (int)x, (int)y - 12, filled, 6 };

	SDL_SetRenderDrawColor(app.renderer, 80, 0, 0, 255);
	SDL_RenderFillRect(app.renderer, &back);
	SDL_SetRenderDrawColor(app.renderer, 255, 40, 40, 255);
	SDL_RenderFillRect(app.renderer, &front);
}