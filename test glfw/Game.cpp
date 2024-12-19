#include <windows.h>
#include <commdlg.h> 
#include "Game.h"
#include "stb_image.h"


//unsigned char* data;
unsigned int texture;

unsigned int VBO, VAO, EBO;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"
Game* Game::instance = nullptr;
float z = 0.0f;
float vertices[] = {
    // positions         // texture coords
     1.0f,  1.0f, z,  1.0f, 1.0f, // top right
     1.0f, -1.0f, z,  1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, z,  0.0f, 0.0f, // bottom left
    -1.0f, 1.0f, z,  0.0f, 1.0f  // top left 
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Game::Game() {
    
    instance = this;

}
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) // gerer mieux
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //old
{
    /*if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        for (int i = 1000; i < 15000; i++) {
            canva.data[i * 3] = 0;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, canva.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }*/
}



void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse) {

        
        drawing = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << xpos << " " << ypos << std::endl;
        last_mouse_x = xpos;
        last_mouse_y = ypos;
        switch (canva.tool)
        {
        case 0:
            return;
        case 1:
            canva.draw_brush(xpos, ypos);
            break;
        case 2:
            canva.draw_brush(xpos, ypos, true);
            break;
        case 3:
            canva.fill(xpos, ypos);
            break;
        case 4: // Cercle
            canva.draw_circle(xpos, ypos, canva.size);
            break;
        }


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canva.width, canva.height, 0, GL_RGB, GL_UNSIGNED_BYTE, canva.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        drawing = false;
    }
}


void Game::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (drawing && !io.WantCaptureMouse) {
        //std::cout << xpos << " " << ypos << std::endl;

        double step_number = std::max(abs(last_mouse_x - xpos),abs(last_mouse_y-ypos));
        switch (canva.tool)
        {
        case 0 :
            return;
        case 1 : // interpolation linéaire entre la dernier position de la souris et la position actuelle mix; (x, y, a) retourne : x*(1-a) + y*a
            for (int a = 0; a < step_number; a++) {
                canva.draw_brush(glm::mix(xpos, last_mouse_x, a / step_number), glm::mix(ypos, last_mouse_y, a / step_number));
            }
            last_mouse_x = xpos;
            last_mouse_y = ypos;
        break;
        case 2:
            for (int a = 0; a < step_number; a++) {
                canva.draw_brush(glm::mix(xpos, last_mouse_x, a / step_number), glm::mix(ypos, last_mouse_y, a / step_number),  true);
            }
            last_mouse_x = xpos;
            last_mouse_y = ypos;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canva.width, canva.height, 0, GL_RGB, GL_UNSIGNED_BYTE, canva.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Game::mouse_button_callback_wrapper(GLFWwindow* window, int button, int action, int mods) {
    if (instance) {
        instance->mouse_button_callback(window, button, action, mods);
    }
}

void Game::cursor_position_callback_wrapper(GLFWwindow* window, double xpos, double ypos) {
    if (instance) {
        instance->cursor_position_callback(window, xpos, ypos);
    }
}

void  Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    z *= 0.9;
}

void Game::scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset) {
    if (instance) {
        instance->scroll_callback(window, xoffset, yoffset);
    }
}

int Game::init_opengl_glfw()
{
    //DEBUT initialisation de glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(800, 600, "Paint RV pour de vrai", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    //setting the glfw callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback_wrapper);
    glfwSetCursorPosCallback(window, cursor_position_callback_wrapper);
    glfwSetScrollCallback(window, scroll_callback_wrapper);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    //FIN

    // initialisation imgui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

int Game::load_texture()
{



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //tell opengl how to use the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    //changer les parametres du texture2D
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load a texture

    canva.data = stbi_load("container.bmp", &(canva.width), &(canva.height), &(canva.nrChannels), 0);
    std::cout << canva.height << canva.width << std::endl;
    if (canva.data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canva.width, canva.height, 0, GL_RGB, GL_UNSIGNED_BYTE, canva.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }    // gen texture
}

int Game::render()
{


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();

    return 0;
}

void Game::init_ui()
{
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    io = ImGui::GetIO();

    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(); // Show demo window! :)

    ImGui::Begin("Tools!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("This is the tools windows.");
    //tool selector
    ImGui::RadioButton("No tool", &canva.tool, 0); ImGui::SameLine();
    ImGui::RadioButton("Brush", &canva.tool, 1); ImGui::SameLine();
    ImGui::RadioButton("Eraser", &canva.tool, 2); ImGui::SameLine();
    ImGui::RadioButton("Fill", &canva.tool, 3);
    ImGui::RadioButton("Circle", &canva.tool, 4);
    ImGui::SliderInt("Taille", &(canva.size), 1, 100);
    //ImGui::SliderFloat("Taille", &size, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("Couleur pinceau", (float*)&(canva.couleur_pinceau)); // Edit 3 floats representing a color


    
    ImGui::End();


    //test de menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            ImGui::MenuItem("(file menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) { printf("New file\n"); }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.jpg");
                ImGui::MenuItem("fish.png");
                ImGui::MenuItem("camarchepas.jpg");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { printf("Save\n"); }
            if (ImGui::MenuItem("Save As..")) { printf("Save as\n"); }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    
  

}


