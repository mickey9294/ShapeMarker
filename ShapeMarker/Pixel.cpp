#include "Pixel.h"



Pixel::Pixel()
{
	x = 0;
	y = 0;
}

Pixel::Pixel(float _x, float _y)
{
	x = _x;
	y = _y;
}

Pixel::Pixel(const Pixel & another)
{
	x = another.x;
	y = another.y;
}


Pixel::~Pixel()
{
}
