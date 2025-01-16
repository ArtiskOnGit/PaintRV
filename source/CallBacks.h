#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class CallBacks
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

