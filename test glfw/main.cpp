
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
    
    game.run();

    game.cleanup();
    

    return 0;
}