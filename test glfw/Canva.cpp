#include "Canva.h"

#ifndef STB_HEADER
#define STB_HEADER


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

const int NR_CHANNEL_WITH_ALPHA = 4;
const int NR_CHANNEL_WITHOUT_ALPHA = 3;





//marche pas pour des grandes surfaces
//void Canva::recursive_fill(int x, int y)
//{
//    if (data[coord_to_indextexture(x, y) * nrChannels] == ColorToFill[0]
//        && data[coord_to_indextexture(x, y) * nrChannels + 1] == ColorToFill[1]
//        && data[coord_to_indextexture(x, y) * nrChannels + 2] == ColorToFill[2]) {
//
//        //std::cout << (255 * couleur_pinceau.x) <<std::endl;
//        data[coord_to_indextexture(x , y ) * nrChannels  ] = (unsigned char)(255 * couleur_pinceau.x);
//        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur_pinceau.y);
//        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur_pinceau.z);
//
//        if (0 <= x && x < width) {
//            recursive_fill(x - 1, y);
//            recursive_fill(x + 1, y);
//        }
//        if (0 <= y && y < height) {
//            recursive_fill(x , y - 1);
//            recursive_fill(x , y + 1);
//        }
//    }
//}






void Canva::actualise_viewport()
{
    glViewport(0, 0, (int) (zoom * width), (int) (zoom * height));
}



void Canva::dessiner_brosse_carree(int xpos_mouse, int ypos_mouse)
{
    if (!gomme) {

        calques[calque_selectionne]->dessiner_brosse_carree(xpos_mouse, ypos_mouse, size, u32_couleur_pinceau);
    }
    else { calques[calque_selectionne]->dessiner_brosse_carree(xpos_mouse, ypos_mouse, size, u32_couleur_eraser); }
    
    
    
    calques[calque_selectionne]->actualise_texture();
}


void Canva::dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse)
{
    ImVec4 couleur;
    if (!gomme) {
        calques[calque_selectionne]->dessiner_brosse_circulaire(xpos_mouse, ypos_mouse, size, u32_couleur_pinceau);
    }
    else { calques[calque_selectionne]->dessiner_brosse_circulaire(xpos_mouse, ypos_mouse, size, u32_couleur_eraser); }

    
    calques[calque_selectionne]->actualise_texture();
    
}


void Canva::fill(int x, int y) {
    calques[calque_selectionne]->fill(x, y, u32_couleur_pinceau);
    calques[calque_selectionne]->actualise_texture();
}


void Canva::draw_circle(int center_x, int center_y, int radius, bool erase) {
    ImU32 couleur;
    if (gomme) {
        couleur = u32_couleur_pinceau;
    }
    else { couleur = u32_couleur_eraser; }

    calques[calque_selectionne]->draw_circle(center_x, center_y, radius, couleur);
    calques[calque_selectionne]->actualise_texture();
}

void Canva::pipette(int x, int y)
{
    u32_couleur_pinceau = (calques[calque_selectionne]->getR(x, y) >> IM_COL32_R_SHIFT & 0xff);
    u32_couleur_pinceau |= (calques[calque_selectionne]->getG(x, y) << IM_COL32_G_SHIFT);
    u32_couleur_pinceau |= (calques[calque_selectionne]->getB(x, y) << IM_COL32_B_SHIFT);
    if (calques[calque_selectionne]->has_alpha) { u32_couleur_pinceau |= (calques[calque_selectionne]->getA(x, y) << IM_COL32_A_SHIFT); }
    else { u32_couleur_pinceau |= (255 << IM_COL32_A_SHIFT); }
}

int Canva::new_blank_canva(int width_canva, int height_canva, bool has_alpha_canva)
{
    //create blank canva
    calques.clear();
    /*if (data) { delete[] data; }
    data = nullptr;*/
    
    

    int nr_channel;
    if (has_alpha_canva) {
        nr_channel = NR_CHANNEL_WITH_ALPHA;
    }
    else { nr_channel = NR_CHANNEL_WITHOUT_ALPHA; }
    
    Calque* new_cal = new Calque(width_canva, height_canva, nr_channel);

    if (new_cal != nullptr) {
        calques.push_back(new_cal);
        height = height_canva;
        width = width_canva;
        has_alpha = has_alpha_canva;
        nombre_calques = 1;
        actualise_viewport();
        calques[0]->actualise_texture();
        //nrChannels = nr_channel;
    }
    else
    {
        std::cout << "Failed to create new texture" << std::endl;
        return -1;
    }    // gen texture



}

int Canva::load_image(const char* filepath)
{
    {
        //load an image
        calques.clear();
        Calque* new_cal = new Calque(filepath);

        
        
       
        if (new_cal)
        {
            calques.push_back(new_cal);
            height = calques[0]->height;
            width = calques[0]->width;
            nombre_calques = 1;

            actualise_viewport();
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
    stbi_write_png(filepath, width, height, calques[calque_selectionne]->nrChannels, calques[calque_selectionne]->data, width * calques[calque_selectionne]->nrChannels);
}
    else if (format == 1) {
        stbi_write_jpg(filepath, width, height, calques[calque_selectionne]->nrChannels, calques[calque_selectionne]->data, 100);
    }
    return true;
}

//void Canva::draw_pixel_at(int x, int y, bool use_couleur_pinceau, ImVec4 couleur)
//{
//    if (use_couleur_pinceau) {
//        data[coord_to_indextexture(x, y) * nrChannels] = (unsigned char)(255 * couleur_pinceau.x);
//        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur_pinceau.y);
//        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur_pinceau.z);
//        if (has_alpha) { data[coord_to_indextexture(x, y) * nrChannels + 3] = (unsigned char)(255 * couleur_pinceau.w); }
//    }
//    else {
//        data[coord_to_indextexture(x, y) * nrChannels] = (unsigned char)(255 * couleur.x);
//        data[coord_to_indextexture(x, y) * nrChannels + 1] = (unsigned char)(255 * couleur.y);
//        data[coord_to_indextexture(x, y) * nrChannels + 2] = (unsigned char)(255 * couleur.z);
//        if (has_alpha) { data[coord_to_indextexture(x, y) * nrChannels + 3] = (unsigned char)(255 * couleur.w); }
//    }
//
//}
