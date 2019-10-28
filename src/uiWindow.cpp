#include <uiWindow.h>

UIWindow::UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color):
    coords(coords),
    width(width),
    height(height),
    color(color),
    last_clicked(glfwGetTime())
{
    setup();
}


void UIWindow::setup()
{
    float vertices[] = {
        //pos         //tex coords
        -1, -1, 0.0f, 0.0f, 0.0f,
         1, -1, 0.0f, 1.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1, -1, 0.0f, 0.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1,  1, 0.0f, 0.0f, 1.0f
    };

        // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // // load textures 
    // texture = loadTexture(("/home/nikita/Code/world_gen/resources/"+filename+".png").c_str());
}
