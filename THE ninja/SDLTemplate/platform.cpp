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

void platform::start()
{
	texture = loadTexture("assets/platform.png");
}

void platform::draw()
{
	SDL_Rect r = { x, y, width, height };
	SDL_RenderCopy(app.renderer, texture, NULL, &r);
}

void platform::update()
{
}
