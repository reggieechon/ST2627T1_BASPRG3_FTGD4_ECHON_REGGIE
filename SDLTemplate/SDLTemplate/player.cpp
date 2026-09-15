#include "Player.h"
#include "GameScene.h"
#include "GameObject.h"

Player::~Player()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}

	bullets.clear();
}

void Player::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/player.png");
	sound = SoundManager::loadSound("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");

	// Initialize to avoid garbage values
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	defaultSpeed = 5;
	boostedSpeed = 10;
	currentSpeed = defaultSpeed;

	reloadTime = 8; // 0.16sec (8/60)
	altreloadTime = 16;
	currentReloadTime = 0;
	altcurrentReloadTime = 0;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Player::update()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->GetX() > SCREEN_WIDTH)
		{
			Bullet* bulletToDelete = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToDelete;

			break;
		}
	}

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}

	if (altcurrentReloadTime > 0)
	{
		altcurrentReloadTime--;
	}

	if (app.keyboard[SDL_SCANCODE_F] && currentReloadTime <= 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet
		(
			x + width - 5,
			y + (height / 2) - 5,
			1,
			0,
			5
		);
		getScene()->addGameObject(bullet);
		bullets.push_back(bullet);

		currentReloadTime = reloadTime;
	}

	if (app.keyboard[SDL_SCANCODE_G] && altcurrentReloadTime <= 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet1 = new Bullet(
			x + width + 5,
			y + height - 5,
			1,
			0,
			5
		);
		Bullet* bullet2 = new Bullet(
			x + width - 5,
			y - 5,
			1,
			0,
			5
		);
		getScene()->addGameObject(bullet1);
		bullets.push_back(bullet1);
		getScene()->addGameObject(bullet2);
		bullets.push_back(bullet2);

		altcurrentReloadTime = altreloadTime;
	}

	if (app.keyboard[SDL_SCANCODE_LSHIFT])
	{
		currentSpeed = boostedSpeed;
	}
	if (app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		currentSpeed = defaultSpeed;
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
}

void Player::draw()
{
	blit(texture, x, y);
}