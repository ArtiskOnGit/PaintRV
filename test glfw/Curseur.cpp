#include "Curseur.h"






Curseur::Curseur() : shader("C:\\Users\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\vertex _curseur.vs", "C:\\Users\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\fragment_curseur.fs") {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);


    //tell opengl how to use the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Curseur::actualise(int xpos, int ypos){

    shader.setFloat("xcurseur", xpos);
    shader.setFloat("ycurseur", ypos);
}


void Curseur::resize_curseur(int width, int height, int size)
{
    float wsize = size * width / 2.f;
    float hsize = size * height / 2.f;
    float vertices[24] = {
        // positions         // texture coords
         wsize,  hsize, 0.0f,  0.1f, 0.2f, 0.2f, // top right
         wsize, -hsize, 0.0f,  0.1f, 0.2f, 0.2f, // bottom right
        -wsize, -hsize, 0.0f,  0.1f, 0.2f, 0.2f, // bottom left
        -wsize,  hsize, 0.0f,  0.1f, 0.2f, 0.2f  // top left
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

}
