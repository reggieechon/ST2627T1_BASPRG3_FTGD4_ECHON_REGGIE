#include "boss.h"
#include "GameScene.h"
#include <cmath>

Boss::Boss(Player* player, int tier, int xPos, int yPos)
{
	this->player = player;
	this->tier = tier;
	x = xPos;
	y = yPos;
}

void Boss::start()
{
	texture = loadTexture("assets/boss.png");
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	width *= 1.5;
	height *= 1.5;

	int hp[] = { 0, 20, 35, 50 };
	maxHealth = hp[tier];
	health = maxHealth;

	entering = true;
	targetY = 60;
	moveDir = 1;
	moveSpeed = 1.5f + tier;

	aimedTimer = 60;
	radialTimer = 150;
}

void Boss::update()
{

	if (entering)
	{
		y += 2;
		if (y >= targetY)
		{
			y = targetY;
			entering = false;
		}
		return;
	}

	x += moveDir * moveSpeed;

	if (x < 0)
	{
		x = 0; moveDir = 1;
	}
	if (x > SCREEN_WIDTH - width)
	{
		x = SCREEN_WIDTH - width; moveDir = -1;
	}

	if (!player->getIsAlive()) return;

	bool enraged = health <= maxHealth / 2;

	aimedTimer--;
	if (aimedTimer <= 0)
	{
		int count = (tier == 1) ? 1 : 3;
		fireAimed(count, 0.25f);
		aimedTimer = enraged ? 40 : 70;
	}

	if (tier >= 2)
	{
		radialTimer--;
		if (radialTimer <= 0)
		{
			fireRadial(tier == 2 ? 10 : 16);
			radialTimer = enraged ? 90 : 150;
		}
	}
}

void Boss::draw()
{
	const float scale = 2.0f;

	SDL_Rect size;

	size.x = (int)x;
	size.y = (int)y;
	size.w = (int)(width * scale);
	size.h = (int)(height * scale);

	SDL_RenderCopy(app.renderer, texture, NULL, &size);

	int barWidth = width;
	int filled = (int)((float)health / maxHealth * barWidth);

	SDL_Rect back = { (int)x, (int)y - 12, barWidth, 6 };
	SDL_Rect front = { (int)x, (int)y - 12, filled, 6 };

	SDL_SetRenderDrawColor(app.renderer, 80, 0, 0, 255);
	SDL_RenderFillRect(app.renderer, &back);
	SDL_SetRenderDrawColor(app.renderer, 255, 40, 40, 255);
	SDL_RenderFillRect(app.renderer, &front);
}

bool Boss::takeDamage(int amount)
{
	health -= amount;
	return health <= 0;
}

void Boss::fireAimed(int count, float spread)
{
	float startX = x + width / 2;
	float startY = y + height;

	float dx = (player->getX() + player->getWidth() / 2) - startX;
	float dy = (player->getY() + player->getHeight() / 2) - startY;
	float baseAngle = atan2f(dy, dx);

	for (int i = 0; i < count; i++)
	{
		float offset = (i - (count - 1) / 2.0f) * spread;
		float angle = baseAngle + offset;

		getScene()->addGameObject(new projectile(startX, startY, cosf(angle), sinf(angle), 4, Side::BOSS_SIDE));
	}
}

void Boss::fireRadial(int count)
{
	float startX = x + width / 2;
	float startY = y + height / 2;

	for (int i = 0; i < count; i++)
	{
		float angle = i * (2.0f * 3.14159f / count);

		getScene()->addGameObject(
			new projectile(startX, startY, cosf(angle), sinf(angle), 3, Side::ENEMY_SIDE));
	}
}