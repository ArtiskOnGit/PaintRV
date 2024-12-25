#pragma once

#include <iostream>

class Texture
{
private:
	bool has_alpha;
	int height, width, channels = 0;
	unsigned char* data = nullptr;

public:
	void dessiner_brosse_carree(int xpos_mouse, int ypos_mouse);
	void dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse);
	void fill(int x, int y);

	Texture(int width_calque, int height_calque, int nr_channels);
};

