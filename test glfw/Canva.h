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
#include "Calque.h"



class Canva
{
private:
	//void recursive_fill(int x, int y);
	//void heap_fill(int x, int y);
	unsigned char ColorToFill[3] = { 255, 255, 255 };

public:
	bool gomme = false;
	int tool = 0;
	int width = 550;
	int height = 550;
	//ImVec4 couleur_pinceau = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImU32 u32_couleur_pinceau = IM_COL32(123, 231, 031, 255);
	//ImVec4 couleur_eraser = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	ImU32 u32_couleur_eraser = IM_COL32(255, 255, 255, 0);

	int size = 3;

	float zoom = 1.0f;

	Canva() {}
	bool has_alpha = false;

	//shared_ptr comme ça quand on calques.clear(), les "Calque" sont automatiquement supprimés
	std::vector<std::shared_ptr<Calque>> calques; // tableau de calques 
	//unsigned char* data = nullptr; //sera un pointeur vers le calque courant
	int calque_selectionne = 0;
	int nombre_calques = 0;

	

	void actualise_viewport();
	//void actualise_texture();

	void dessiner_brosse_carree(int xpos_mouse, int ypos_mouse);
	void dessiner_brosse_circulaire(int xpos_mouse, int ypos_mouse);
	void fill(int x, int y);

	void draw_circle(int center_x, int center_y, int radius, bool erase = false);
	void pipette(int x, int y);

	int new_blank_canva(int width_canva, int height_canva, bool has_alpha);
	int load_image(const char* filepath);
	int nouveau_calque();
	bool save_image(const char* filepath, int format);
	//void draw_pixel_at(int x, int y, bool use_couleur_pinceau = true, ImVec4 couleur = ImVec4(1.f, 1.f, 1.f, 0.00f));

};

