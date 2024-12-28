#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp> 

#include "Shader.h"


class Curseur
{
private:
    Shader shader;
	unsigned int VAO, VBO, EBO;
    float vertices[24] = {
        // positions         // texture coords
         1.0f,  1.0f, 0.0f,  0.1f, 0.2f, 0.2f, // top right
         1.0f, -1.0f, 0.0f,  0.1f, 0.2f, 0.2f, // bottom right
        -1.0f, -1.0f, 0.0f,  0.1f, 0.2f, 0.2f, // bottom left
        -1.0f, 1.0f,  0.0f,  0.1f, 0.2f, 0.2f, // top left 
    };
    unsigned int indices[6] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };

public:
    Curseur();
	void resize_curseur(int width, int height, int size);
    void actualise(int xpos, int ypos);

};

