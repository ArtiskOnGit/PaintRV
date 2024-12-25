#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include <queue>
#include <utility>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Canva.h"

class Canva
{
private:
	void recursive_fill(int x, int y);
	void heap_fill(int x, int y);
	unsigned char ColorToFill[3] = { 255, 255, 255 };

public:
	bool gomme = false;
	int tool = 0;
	int width = 550;
	int height = 550;
	int nrChannels = 3;
	ImVec4 couleur_pinceau = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 couleur_eraser = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	int size = 3;

	float zoom = 1.0f;

	Canva() {}
	unsigned char* data = nullptr;

	void actualise_viewport();

	void dessiner_brosse_carree(int xpos_mouse, int ypos_mouse);
	void dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse);
	void fill(int x, int y);
	int coord_to_indextexture(int x, int y);
	void draw_circle(int center_x, int center_y, int radius, bool erase = false);
	void pipette(int x, int y);

	int new_blank_canva(int width_canva, int height_canva);
	int load_image(const char* filepath);
	bool save_image(const char* filepath, int format);
	void draw_pixel_at(int x, int y, bool use_couleur_pinceau = true, ImVec4 couleur = ImVec4(1.f, 1.f, 1.f, 1.00f));

};

