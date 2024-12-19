#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp> 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Canva.h"
#include "ImguiWindows.h"

#include "Shader.h"

#include <iostream>
#include <algorithm>
#include <iostream>
class Game
{
private :
	bool drawing = false;
	double last_mouse_x = 0;
	double last_mouse_y = 0;

	int window_height;
	int window_width;

	
public:
	ImguiWindows* imguiWindows;
	Canva canva;
	GLFWwindow* window = nullptr;
	static Game* instance;
	int init_opengl_glfw();
	void prepare_vertex();
	int new_blank_canva(int width_canva, int height_canva);
	int load_image(const char* filepath);
	int render();
	Game();

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_position_callback_wrapper(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

};

