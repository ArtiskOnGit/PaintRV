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
	int tool;
	int width, height, nrChannels;
	ImVec4 couleur_pinceau = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 couleur_eraser = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	int size = 3;

	Canva() {}
	unsigned char* data;

	void draw_brush(int xpos_mouse, int ypos_mouse, bool eraser = false);
	void fill(int x, int y);
	int coord_to_indextexture(int x, int y);
	void draw_circle(double center_x, double center_y, int radius, bool erase = false);

};

