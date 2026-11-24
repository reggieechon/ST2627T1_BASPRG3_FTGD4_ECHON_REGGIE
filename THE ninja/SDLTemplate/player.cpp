#include "player.h"
#include "GameScene.h"
#include "GameObject.h"
#include "platform.h"
#include "Scene.h"
#include <cmath>

using namespace std;

const int hbOffsetX = 12;
const int hbOffsetY = 9;
const int hbInsetX = 24;

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

	int hp[] = { 20, 40, 60, 80 };
	maxHealth = hp[tier];
	health = maxHealth;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	x = (SCREEN_WIDTH / 2) - (width / 2);
	y = SCREEN_HEIGHT - height - 50;
	velocityY = 0;
	onGround = false;
}

SDL_Rect Player::getHitBox()
{
	SDL_Rect player;

	player.x = (int)x + hbOffsetX;
	player.y = (int)y + hbOffsetY;
	player.w = width - hbInsetX;
	player.h = height - hbOffsetY;

	return player;
}

void Player::update()
{
	if (tpCooldown > 0)
	{
		tpCooldown--;
	}

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
		texture = loadTexture("assets/player_left.png");
		dx -= currentSpeed;
	}

	if (app.keyboard[SDL_SCANCODE_D]) 
	{
		texture = loadTexture("assets/player_right.png");
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

	if (app.keyboard[SDL_SCANCODE_F] && tpCooldown <= 0)
	{
		x = app.mouseX - width / 2;
		y = app.mouseY - height / 2;
		velocityY = 0;
		tpCooldown = 5;
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] && onGround)
	{
		SoundManager::playSound(jump);
		velocityY = -jumpStrength;
	}

	//what am i doin man
	float prevBot = static_cast<float>(y + height);
	velocityY += gravity;

	if (velocityY > fallSpeed) 
	{
		velocityY = fallSpeed;
	}

	y += velocityY;
	onGround = false;

	float curBot = static_cast<float>(y + height);
	SDL_Rect hb = getHitBox();

	if(velocityY > 0)
	{
		for (GameObject* obj : getScene()->getObjects())
		{
			platform* p = dynamic_cast<platform*>(obj);
			if (p == nullptr)  continue;

			bool overlapX = hb.x < p->getX() + p->getWidth() && hb.x + hb.w > p->getX();
			if (!overlapX) continue;

			float platformTop = static_cast<float>(p->getY());

			if (prevBot <= platformTop && curBot >= platformTop)
			{
				y = static_cast<float>(platformTop) - height;
				velocityY = 0.0f;
				onGround = true;
				break;
			}
		}
	}

	if (x < - hbOffsetX) x = -hbInsetX;
	if (x > SCREEN_WIDTH - width + (hbInsetX - hbOffsetX)) x = SCREEN_WIDTH - width + (hbInsetX - hbOffsetX) ;
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

		int barWidth = width;
		int filled = (int)((float)health / maxHealth * barWidth);

		SDL_Rect back = { (int)x, (int)y - 12, barWidth, 6 };
		SDL_Rect front = { (int)x, (int)y - 12, filled, 6 };

		SDL_SetRenderDrawColor(app.renderer, 80, 0, 0, 255);
		SDL_RenderFillRect(app.renderer, &back);
		SDL_SetRenderDrawColor(app.renderer, 0, 128, 0, 255);
		SDL_RenderFillRect(app.renderer, &front);
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

bool Player::takeDamage(int amount)
{
	health -= amount;

	if (health <= 0)
	{
		health = 0;
		isAlive = false;
		return true;
	}

	return false;
}

//i miss you unity

