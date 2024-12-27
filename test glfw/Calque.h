#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <queue>
#include <utility>



class Calque
{
private:
	
public:
	unsigned int texture;
	char nom[128] = "Calque";
	Calque(int new_width, int new_height, int new_channels);
	Calque(const char* filepath);
	void init_texture();
	~Calque() { delete[]data;}
	bool has_alpha = false;
	bool activated = true;
	int width = 550;
	int height = 550;
	int nrChannels = 3;
	unsigned char* data = nullptr;
	void actualise_texture();
	void dessiner_brosse_carree(int xpos_mouse, int ypos_mouse, const int& size, const ImU32& couleur);
	void dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse, const int& size, const ImU32& couleur);
	void draw_circle(int center_x, int center_y, int radius, const ImU32& couleur);
	void fill(int x, int y, ImU32 couleur);
	//void pipette(int x, int y);
	int coord_to_indextexture(int x, int y);

	void draw_pixel_at(int x, int y,const ImU32 &couleur );

	unsigned char getR(int x, int y);
	unsigned char getG(int x, int y);
	unsigned char getB(int x, int y);
	unsigned char getA(int x,int y);

};

