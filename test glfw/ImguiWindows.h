#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp> 

#include "Canva.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class ImguiWindows
{
private : 
	Canva* canva = nullptr;
	int new_canva_width = 550;
	int new_canva_height = 550;
	
public:
	ImGuiIO io;

	ImguiWindows(Canva* c) : canva(c) {}
	void init_ui();

	bool showNewFileWindow = false;
	bool showSaveAsWindow = false;
	void NewFileWindow();
	void SaveAsWindow();
};

