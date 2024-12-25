#include "Texture.h"

void Texture::dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse)
{for (int i = -size ; i < size ; i++) {
        for (int j = -size ; j < size ; j++) {
            if (i * i + j * j <= size * size) {
                draw_pixel_at(xpos_mouse + i, ypos_mouse + j, !gomme);
            }
            /*data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3] = (unsigned char)(255 * couleur.x);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 1] = (unsigned char)(255 * couleur.y);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 2] = (unsigned char)(255 * couleur.z);*/
        }
    }
}

Texture::Texture(int width_calque, int height_calque, int nr_channels)
{
	has_alpha = (channels == 4);
    data = new unsigned char[nr_channels * width_calque * height_calque];
    for (int i = 0; i < nr_channels * width_calque * height_calque; i++) {
        data[i] = 255;
    }

    std::cout << "size of new frame : " << height << "x" << width << "x" << nr_channels << std::endl;
    if (data)
    {
        height = height_calque;
        width = width_calque;
        has_alpha = nr_channels==4;
        channels = nr_channels;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }    // gen texture
}
