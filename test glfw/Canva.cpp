#include "Canva.h"


//marche pas pour des grandes surfaces
void Canva::recursive_fill(int x, int y)
{
    if (data[coord_to_indextexture(x, y) * 3] == ColorToFill[0]
        && data[coord_to_indextexture(x, y) * 3 + 1] == ColorToFill[1]
        && data[coord_to_indextexture(x, y) * 3 + 2] == ColorToFill[2]) {

        //std::cout << (255 * couleur_pinceau.x) <<std::endl;
        data[coord_to_indextexture(x , y ) * 3  ] = (unsigned char)(255 * couleur_pinceau.x);
        data[coord_to_indextexture(x, y) * 3 + 1] = (unsigned char)(255 * couleur_pinceau.y);
        data[coord_to_indextexture(x, y) * 3 + 2] = (unsigned char)(255 * couleur_pinceau.z);

        if (0 < x && x < width) {
            recursive_fill(x - 1, y);
            recursive_fill(x + 1, y);
        }
        if (0 < y && y < height) {
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

        // Vérifier si pixel deja visité
        int index = coord_to_indextexture(xcurr, ycurr);
        if (visited[index]) {
            continue;
        }
        visited[index] = true;

        if (data[coord_to_indextexture(xcurr, ycurr) * 3] == ColorToFill[0]
            && data[coord_to_indextexture(xcurr, ycurr) * 3 + 1] == ColorToFill[1]
            && data[coord_to_indextexture(xcurr, ycurr) * 3 + 2] == ColorToFill[2]) {

            //std::cout << (255 * couleur_pinceau.x) <<std::endl;
            data[coord_to_indextexture(xcurr, ycurr) * 3] = (unsigned char)(255 * couleur_pinceau.x);
            data[coord_to_indextexture(xcurr, ycurr) * 3 + 1] = (unsigned char)(255 * couleur_pinceau.y);
            data[coord_to_indextexture(xcurr, ycurr) * 3 + 2] = (unsigned char)(255 * couleur_pinceau.z);

            if (1 < xcurr && xcurr < width-1) {
                xq.push(xcurr - 1); yq.push(ycurr);
                xq.push(xcurr + 1); yq.push(ycurr);
            }
            if (1 < ycurr && ycurr < height-1) {
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
    y = height - y - 1;
    assert((x + y * width)*3 < width*height*3);
    return  x + y * width;
}

void Canva::draw_brush(int xpos_mouse, int ypos_mouse, bool eraser )
{
    ImVec4 couleur;
    if (eraser) { couleur = couleur_eraser; }
    else { couleur = couleur_pinceau; }
    for (int i = -size / 2; i < size / 2; i++) {
        for (int j = -size / 2; j < size / 2; j++) {

            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3] = (unsigned char)(255 * couleur.x);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 1] = (unsigned char)(255 * couleur.y);
            data[coord_to_indextexture(xpos_mouse + i, ypos_mouse + j) * 3 + 2] = (unsigned char)(255 * couleur.z);
        }
    }
}



void Canva::fill(int x, int y) {
    ColorToFill[0] = data[coord_to_indextexture(x, y) * 3];
    ColorToFill[1] = data[coord_to_indextexture(x, y) * 3 + 1];
    ColorToFill[2] = data[coord_to_indextexture(x, y) * 3 + 2];
    //std::cout << "ColorToFill: "
        //<< (int)ColorToFill[0] << " "
        //<< (int)ColorToFill[1] << " "
        //<< (int)ColorToFill[2] << std::endl;

    heap_fill(x, y);
}


void Canva::draw_circle(double center_x, double center_y, int radius, bool erase) {
    

    // Parcours de la zone autour du centre 
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) { 
                int px = center_x + x;
                int py = center_y + y;

                //modifie la couleur des pixels
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    data[coord_to_indextexture(px, py) * 3] = (unsigned char)(255 * couleur_pinceau.x);
                    data[coord_to_indextexture(px, py) * 3 + 1] = (unsigned char)(255 * couleur_pinceau.y);
                    data[coord_to_indextexture(px, py) * 3 + 2] = (unsigned char)(255 * couleur_pinceau.z);

                }
            }
        }
    }
}

int Canva::new_blank_canva(int width_canva, int height_canva)
{
    //create blank canva
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (data) { delete[] data; }
    data = nullptr;
    height = height_canva;
    width = width_canva;
    glViewport(0, 0, width, height);
    data = new unsigned char[3 * width * height];
    for (int i = 0; i < 3 * width * height; i++) {
        data[i] = 255;
    }
    
    std::cout <<"size of new frame : " << height << " " << width << std::endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }    // gen texture
}