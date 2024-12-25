// test glfw.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include "Shader.h"
#include "Game.h"

#include <iostream>
#include <stdlib.h>
#include <algorithm>


int main()
{
    Game game;
    game.init_opengl_glfw();
    game.prepare_vertex();
    //ouvrir la texture, les vertex
    //game.load_image("container.bmp"); //methode archaïque horrible
    game.canva.new_blank_canva(550, 550, false);

    Shader shader("C:\\Users\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\vertex.vs", "C:\\Users\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\fragment.fs");

    while (!glfwWindowShouldClose(game.window))
    {
        game.imguiWindows->init_ui();
        
        shader.use();
        //shader.setFloat("zoom", game.canva.zoom);

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        game.render();

    }

    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}