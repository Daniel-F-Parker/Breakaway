#pragma once
#include "Vector.h"

struct Ball
{
	float x;
	float y;
	size_t size;
};

struct Paddle
{
	float x;
	float y;
};

struct Brick
{
	SDL_Rect rect;
	int state;
};