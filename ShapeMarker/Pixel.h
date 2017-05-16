#pragma once
class Pixel
{
public:
	Pixel();
	Pixel(float _x, float _y);
	Pixel(const Pixel &another);
	~Pixel();

	float x;
	float y;
};

