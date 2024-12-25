#pragma once
#include "Texture.h"
class Calque
{
	
	int width = 0;
	int height = 0;
	int channels = 0;
	float oppacity = 0;
public:
	Texture* texture = nullptr;
	Calque(int width_calque, int height_calque, int nr_channels);
	~Calque() { delete texture; }
};

