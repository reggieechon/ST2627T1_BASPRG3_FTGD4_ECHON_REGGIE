#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

class platform : public GameObject
{
public:

	SDL_Texture* texture;

	platform(int x, int y, int w, int h);
	void start() override;
	void draw() override;
	void update() override;
};