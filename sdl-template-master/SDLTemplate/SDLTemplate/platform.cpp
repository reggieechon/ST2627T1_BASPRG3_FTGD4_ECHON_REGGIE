#include "platform.h"
#include "GameScene.h"
#include "GameObject.h"

platform::platform(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

void platform::draw()
{
	texture = loadTexture("assets/platform.png");
	blit(texture, x, y);
}

void platform::update()
{
	y += 2;
}
