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



void Canva::draw_square_brush(int xpos_mouse, int ypos_mouse)
{
    if (!eraser) {

        calques[selected_layer]->dessiner_brosse_carree(xpos_mouse, ypos_mouse, size, u32_color_brush);
    }
    else { calques[selected_layer]->dessiner_brosse_carree(xpos_mouse, ypos_mouse, size, u32_color_eraser); }
    
    
    
    //calques[selected_layer]->actualise_texture();
}


void Canva::draw_circle_brush(int xpos_mouse, int ypos_mouse)
{
    ImVec4 couleur;
    if (!eraser) {
        calques[selected_layer]->dessiner_brosse_circulaire(xpos_mouse, ypos_mouse, size, u32_color_brush);
    }
    else { calques[selected_layer]->dessiner_brosse_circulaire(xpos_mouse, ypos_mouse, size, u32_color_eraser); }

    //calques[selected_layer]->actualise_texture();
    
    
}


void Canva::fill(int x, int y) {
    calques[selected_layer]->fill(x, y, u32_color_brush);
    //calques[selected_layer]->actualise_texture();
}


void Canva::draw_circle(int center_x, int center_y, int radius, bool erase) {
    ImU32 couleur;
    if (!eraser) {
        couleur = u32_color_brush;
    }
    else { couleur = u32_color_eraser; }

    calques[selected_layer]->draw_circle(center_x, center_y, radius, couleur);
    calques[selected_layer]->actualise_texture();
}

void Canva::pipette(int x, int y)
{
    u32_color_brush = (calques[selected_layer]->getR(x, y) >> IM_COL32_R_SHIFT & 0xff);
    u32_color_brush |= (calques[selected_layer]->getG(x, y) << IM_COL32_G_SHIFT);
    u32_color_brush |= (calques[selected_layer]->getB(x, y) << IM_COL32_B_SHIFT);
    if (calques[selected_layer]->has_alpha) { u32_color_brush |= (calques[selected_layer]->getA(x, y) << IM_COL32_A_SHIFT); }
    else { u32_color_brush |= (255 << IM_COL32_A_SHIFT); }
}

int Canva::new_blank_canva(int width_canva, int height_canva, bool has_alpha_canva)
{
    //create blank canva
    calques.clear(); //supprimes les anciens calques, shared_ptr donc memoire gérée automatiquement

    int nr_channel;
    if (has_alpha_canva) {
        nr_channel = NR_CHANNEL_WITH_ALPHA;
    }
    else { nr_channel = NR_CHANNEL_WITHOUT_ALPHA; }
    
    std::shared_ptr <Layer> new_cal = std::make_shared<Layer>(width_canva, height_canva, nr_channel);

    if (new_cal != nullptr) {
        selected_layer = 0;
        calques.push_back(new_cal);
        height = height_canva;
        width = width_canva;
        has_alpha = has_alpha_canva;
        nr_layer = 1;
        actualise_viewport();
        calques[0]->actualise_texture();
        return 0;
    }
    else
    {
        std::cout << "Failed to create new canva" << std::endl;
        return -1;
    }    
}

int Canva::load_image(const char* filepath)
{
    {
        //load an image        
        try {
            std::shared_ptr<Layer> new_cal;
            new_cal = std::make_shared<Layer>(filepath);
            selected_layer = 0;
            calques.clear();
            calques.push_back(new_cal); //pushback dans ce cas est une mauvaise pratique je crois, voir https://youtu.be/Xx-NcqmveDc?si=pCQifBdRHjg4BG1E&t=1275
            height = calques[0]->height;
            width = calques[0]->width;
            nr_layer = 1;

            actualise_viewport();
            return 0;
        }
        catch (int err) {
            std::cout << "Failed to load image :" << err << std::endl;
            /*delete new_cal;*/
            return -1;
        }
       
        
    }
}

int Canva::new_layer()
{

    int nr_channel;
    if (has_alpha) {
        nr_channel = NR_CHANNEL_WITH_ALPHA;
    }
    else { nr_channel = NR_CHANNEL_WITHOUT_ALPHA; }

    

    try {
        std::shared_ptr<Layer> new_cal = std::make_shared<Layer>(width, height, nr_channel);
        calques.push_back(new_cal); //pushback dans ce cas est une mauvaise pratique je crois, voir https://youtu.be/Xx-NcqmveDc?si=pCQifBdRHjg4BG1E&t=1275
        nr_layer += 1;
        actualise_viewport();
        calques[nr_layer-1]->actualise_texture();
        
        return 0;
    }
    catch (int err)
    {
        std::cout << "Failed to create new calque" << err << std::endl;
        return -1;
    }   

    return 0;
}

bool Canva::save_image(const char* filepath, int format)
{
    if (format == 0) { //png
    stbi_write_png(filepath, width, height, calques[selected_layer]->nrChannels, calques[selected_layer]->data, width * calques[selected_layer]->nrChannels);
}
    else if (format == 1) {
        stbi_write_jpg(filepath, width, height, calques[selected_layer]->nrChannels, calques[selected_layer]->data, 100);
    }
    return true;
}
