#include "Calque.h"

#ifndef STB_HEADER
#define STB_HEADER
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#endif

const int NR_CHANNEL_WITH_ALPHA = 4;
const int NR_CHANNEL_WITHOUT_ALPHA = 3;

Calque::Calque(int new_width, int new_height, int new_channels)
{   
    init_texture();
	data = new unsigned char[new_width * new_height * new_channels];
	for (int i = 0; i < new_channels * new_width * new_height; i++) {
		data[i] = 255;
	}
    if (data) {
        height = new_height;
        width = new_width;
        nrChannels = new_channels;
        has_alpha = nrChannels == 4;
    }
	std::cout << "size of new calque : " << new_height << "x" << new_width << "x" << new_channels << std::endl;

}

Calque::Calque(const char* filepath)
{
    init_texture();
    if (data) { delete[] data; }
    data = nullptr;
    int image_width, image_height = 0;
    int image_channel = 3;
    data = stbi_load(filepath, &(image_width), &(image_height), &(image_channel), 0);

    if (data)
    {
        height = image_height;
        width = image_width;
        has_alpha = (image_channel == NR_CHANNEL_WITH_ALPHA);
        nrChannels = image_channel;

        actualise_texture();
    }
    else
    {
        std::cout << "Failed to load image" << std::endl;
    }   
}

void Calque::init_texture()
{
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    //changer les parametres du texture2D
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //tell opengl what to do with the alphas channels
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Calque::fill(int x, int y, ImU32 couleur)
{
    unsigned char ColorToFill[4] = { getR(x, y), getG(x, y), getB(x, y) , 255};
    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited(width * height, false);
    q.push({ x, y });

    std::pair<int, int> curr;
    while (!q.empty()) {


        curr = q.front();
        int xcurr = curr.first;
        int ycurr = curr.second;
        q.pop();


        if (xcurr < 0 || xcurr >= width || ycurr < 0 || ycurr >= height) {
            continue;
        }

        // V�rifier si pixel deja visit�
        int index = coord_to_indextexture(xcurr, ycurr);
        if (visited[index]) {
            continue;
        }
        visited[index] = true;

        if (getR(xcurr, ycurr) == ColorToFill[0]
            && getG(xcurr, ycurr) == ColorToFill[1]
            && getB(xcurr, ycurr) == ColorToFill[2]) {

            draw_pixel_at(xcurr, ycurr, couleur);

            if (1 <= xcurr && xcurr < width - 1) {
                q.push({ xcurr - 1 , ycurr });
                q.push({ xcurr + 1 , ycurr });

            }
            if (1 <= ycurr && ycurr < height - 1) {
                q.push({ xcurr , ycurr - 1 });
                q.push({ xcurr , ycurr + 1 });

            }

        }

    }


}
int Calque::coord_to_indextexture(int x, int y) {
    if (x >= width) { x = width - 1; }
    if (y >= height) { y = height - 1; }
    if (x < 0) { x = 0; }
    if (y < 0) { y = 0; }
    //y = height - y - 1;
    //std::cout << "ici" << height << width << std::endl;
    assert((x + y * width) * nrChannels < width * height * nrChannels);
    return  x + y * width;
}

void Calque::draw_pixel_at(int x, int y,const ImU32& couleur)
{
    int index = coord_to_indextexture(x, y);
    data[index * nrChannels] = (couleur >> IM_COL32_R_SHIFT & 0xff); //R
    data[index * nrChannels + 1] = (couleur >> IM_COL32_G_SHIFT & 0xff); //G
    data[index * nrChannels + 2] = (couleur >> IM_COL32_B_SHIFT & 0xff); //B
    if (has_alpha) {
        data[index * nrChannels + 3] = (couleur >> IM_COL32_A_SHIFT & 0xff); //A
    }
}

void Calque::actualise_texture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    if (has_alpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
}

void Calque::dessiner_brosse_carree(int xpos_mouse, int ypos_mouse, const int& size, const ImU32& couleur)
{
   
    //std::cout << "couleur : " << (couleur >> IM_COL32_R_SHIFT & 0xff) << " " << (couleur >> IM_COL32_G_SHIFT & 0xff) << " " << (couleur >> IM_COL32_B_SHIFT & 0xff) << std::endl;
    
    for (int i = -size / 2; i < size / 2; i++) {
        for (int j = -size / 2; j < size / 2; j++) {
            draw_pixel_at(xpos_mouse + i, ypos_mouse + j, couleur);
        }
    }
}

void Calque::dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse, const int& size, const ImU32& couleur)
{
    for (int i = -size; i < size; i++) {
        for (int j = -size; j < size; j++) {
            if (i * i + j * j <= size * size) {
                draw_pixel_at(xpos_mouse + i, ypos_mouse + j, couleur);
            }
        }
    }
}

void Calque::draw_circle(int center_x, int center_y, int radius, const ImU32& couleur)
{
    // Parcours de la zone autour du centre 
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                int px = center_x + x;
                int py = center_y + y;

                //modifie la couleur des pixels
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    draw_pixel_at(px, py, couleur);

                }
            }
        }
    }
}


unsigned char Calque::getR(int x, int y)
{
    return data[coord_to_indextexture(x, y)*nrChannels];
}
unsigned char Calque::getG(int x, int y)
{
    return data[coord_to_indextexture(x, y) * nrChannels + 1];
}
unsigned char Calque::getB(int x, int y)
{
    return data[coord_to_indextexture(x, y) * nrChannels + 2];
}
unsigned char Calque::getA(int x, int y)
{
    return data[coord_to_indextexture(x, y) * nrChannels + 3];
}

