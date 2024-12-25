#include "Canva.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int NR_CHANNEL_WITH_ALPHA = 4;
const int NR_CHANNEL_WITHOUT_ALPHA = 3;




//marche pas pour des grandes surfaces
void Canva::recursive_fill(int x, int y)
{
    if (data[coord_to_indextexture(x, y) * nrChannels] == ColorToFill[0]
        && data[coord_to_indextexture(x, y) * nrChannels + 1] == ColorToFill[1]
        && data[coord_to_indextexture(x, y) * nrChannels + 2] == ColorToFill[2]) {

        //std::cout << (255 * couleur_pinceau.x) <<std::endl;
        data[coord_to_indextexture(x , y ) * nrChannels  ] = (unsigned char)(255 * couleur_pinceau.x);
        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur_pinceau.y);
        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur_pinceau.z);

        if (0 <= x && x < width) {
            recursive_fill(x - 1, y);
            recursive_fill(x + 1, y);
        }
        if (0 <= y && y < height) {
            recursive_fill(x , y - 1);
            recursive_fill(x , y + 1);
        }
    }
}

void Canva::heap_fill(int x, int y)
{
    std::queue<int> xq;
    std::queue<int> yq;
    std::vector<bool> visited(width * height, false);
    xq.push(x);
    yq.push(y);
    int xcurr;
    int ycurr;
    while(!xq.empty()) {

        xcurr = xq.front();
        ycurr = yq.front();
        xq.pop();
        yq.pop();

        
        if (xcurr < 0 || xcurr >= width || ycurr < 0 || ycurr >= height) {
            continue;
        }

        // V�rifier si pixel deja visit�
        int index = coord_to_indextexture(xcurr, ycurr);
        if (visited[index]) {
            continue;
        }
        visited[index] = true;

        if (data[coord_to_indextexture(xcurr, ycurr) * nrChannels] == ColorToFill[0]
            && data[coord_to_indextexture(xcurr, ycurr) * nrChannels + 1] == ColorToFill[1]
            && data[coord_to_indextexture(xcurr, ycurr) * nrChannels + 2] == ColorToFill[2]) {

            draw_pixel_at(xcurr, ycurr);

            if (1 <= xcurr && xcurr < width-1) {
                xq.push(xcurr - 1); yq.push(ycurr);
                xq.push(xcurr + 1); yq.push(ycurr);
            }
            if (1 <= ycurr && ycurr < height-1) {
                xq.push(xcurr); yq.push(ycurr - 1);
                xq.push(xcurr); yq.push(ycurr + 1);
            }

        }
   
    }
}



int Canva::coord_to_indextexture(int x, int y) {
    if (x >= width) { x = width-1; }
    if (y >= height) { y =  height - 1; }
    if (x < 0) { x = 0; }
    if (y < 0) { y = 0; }
    //y = height - y - 1;
    assert((x + y * width)*3 < width*height*3);
    return  x + y * width;
}

void Canva::actualise_viewport()
{
    glViewport(0, 0, (int) (zoom * width), (int) (zoom * height));
}

void Canva::actualise_texture()
{
    if (has_alpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
}

void Canva::dessiner_brosse_carree(int xpos_mouse, int ypos_mouse)
{
    ImVec4 couleur;
    //if (eraser) { couleur = couleur_eraser; }
    //else { couleur = couleur_pinceau; }
    for (int i = -size / 2; i < size / 2; i++) {
        for (int j = -size / 2; j < size / 2; j++) {
            draw_pixel_at(xpos_mouse + i, ypos_mouse + j, !gomme);
            /*data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3] = (unsigned char)(255 * couleur.x);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 1] = (unsigned char)(255 * couleur.y);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 2] = (unsigned char)(255 * couleur.z);*/
        }
    }
}


void Canva::dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse)
{
    ImVec4 couleur;
    //if (eraser) { couleur = couleur_eraser; }
    //else { couleur = couleur_pinceau; }
    for (int i = -size ; i < size ; i++) {
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


void Canva::fill(int x, int y) {
    ColorToFill[0] = data[coord_to_indextexture(x, y) * nrChannels];
    ColorToFill[1] = data[coord_to_indextexture(x, y) * nrChannels + 1];
    ColorToFill[2] = data[coord_to_indextexture(x, y) * nrChannels + 2];
    //std::cout << "ColorToFill: "
        //<< (int)ColorToFill[0] << " "
        //<< (int)ColorToFill[1] << " "
        //<< (int)ColorToFill[2] << std::endl;

    heap_fill(x, y);
}


void Canva::draw_circle(int center_x, int center_y, int radius, bool erase) {
    

    // Parcours de la zone autour du centre 
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) { 
                int px = center_x + x;
                int py = center_y + y;

                //modifie la couleur des pixels
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    draw_pixel_at(px, py);

                }
            }
        }
    }
}

void Canva::pipette(int x, int y)
{
    couleur_pinceau.x = ((float) data[coord_to_indextexture(x, y) * nrChannels] + 0)/255;
    couleur_pinceau.y = ((float) data[coord_to_indextexture(x, y) * nrChannels + 1])/255;
    couleur_pinceau.z = ((float) data[coord_to_indextexture(x, y) * nrChannels + 2])/255;
}

int Canva::new_blank_canva(int width_canva, int height_canva, bool has_alpha_canva)
{
    //create blank canva
    
    if (data) { delete[] data; }
    data = nullptr;

    int nr_channel;
    if (has_alpha_canva) {
        nr_channel = NR_CHANNEL_WITH_ALPHA;
    }
    else { nr_channel = NR_CHANNEL_WITHOUT_ALPHA; }
    
    data = new unsigned char[nr_channel * width_canva * height_canva];
    for (int i = 0; i < nr_channel * width * height; i++) {
        data[i] = 255;
    }
    
    std::cout <<"size of new frame : " << height << "x" << width  << "x" << nr_channel << std::endl;
    if (data)
    {
        height = height_canva;
        width = width_canva;
        has_alpha = has_alpha_canva;
        nrChannels = nr_channel;
        nombre_calques = 1;
        actualise_viewport();
        actualise_texture();

    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }    // gen texture
}

int Canva::load_image(const char* filepath)
{
    {
        //load an image
        if (data) { delete[] data; }
        data = nullptr;
        int image_width, image_height = 0;
        int image_channel = 3;
        data = stbi_load(filepath, &(image_width), &(image_height), &(image_channel), 0);
       
        if (data)
        {
            height = image_height;
            width = image_width;
            has_alpha = (image_channel==NR_CHANNEL_WITH_ALPHA);
            nrChannels = image_channel;
            nombre_calques = 1;

            actualise_viewport();
            actualise_texture();
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load image" << std::endl;
            return -1;
        }    // gen texture
    }
}

bool Canva::save_image(const char* filepath, int format)
{
    if (format == 0) { //png
    stbi_write_png(filepath, width, height, nrChannels, data, width * nrChannels);
}
    else if (format == 1) {
        stbi_write_jpg(filepath, width, height, nrChannels, data, 100);
    }
    return true;
}

void Canva::draw_pixel_at(int x, int y, bool use_couleur_pinceau, ImVec4 couleur)
{
    if (use_couleur_pinceau) {
        data[coord_to_indextexture(x, y) * nrChannels] = (unsigned char)(255 * couleur_pinceau.x);
        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur_pinceau.y);
        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur_pinceau.z);
        if (has_alpha) { data[coord_to_indextexture(x, y) * nrChannels + 3] = (unsigned char)(255 * couleur_pinceau.w); }
    }
    else {
        data[coord_to_indextexture(x, y) * nrChannels] = (unsigned char)(255 * couleur.x);
        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur.y);
        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur.z);
        if (has_alpha) { data[coord_to_indextexture(x, y) * nrChannels + 3] = (unsigned char)(255 * couleur.w); }
    }

}
