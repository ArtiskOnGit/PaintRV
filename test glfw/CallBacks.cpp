#include "CallBacks.h"

static void CallBacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << xpos << " " << ypos << std::endl;

        std::cout << coord_to_indextexture(xpos, ypos) << std::endl;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                data[coord_to_indextexture(xpos + i, ypos + j) * 3] = 0;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}