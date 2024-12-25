#include "Calque.h"

Calque::Calque(int width_calque, int height_calque, int nr_channels)
{
	texture = new Texture(width_calque, height_calque, nr_channels);
	if (texture != nullptr) {
		width = width_calque;
		height = height_calque;
		channels = nr_channels;
	}
}