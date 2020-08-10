#include <skyboxRenderer.h>
#include <misc.h>

// Include standard headers
#include <fstream>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>

SkyboxRenderer::SkyboxRenderer(Settings& settings) :
    settings(settings)
{
    createShaders();
    createVAOVBOtex();
}


void SkyboxRenderer::render(Camera& camera)
{
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skybox_shaderprogram.bind();
    glm::mat4 projView = getProjViewMatrix(camera);
    setProjViewMatrix(projView);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyboxRenderer::createVAOVBOtex()
{
    float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f};

    // skybox VAO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //texture
    std::vector<std::string> faces{
        "/home/nikita/Code/world_gen/resources/arctic_skybox/right.jpg",
        "/home/nikita/Code/world_gen/resources/arctic_skybox/left.jpg",
        "/home/nikita/Code/world_gen/resources/arctic_skybox/top.jpg",
        "/home/nikita/Code/world_gen/resources/arctic_skybox/bottom.jpg",
        "/home/nikita/Code/world_gen/resources/arctic_skybox/front.jpg",
        "/home/nikita/Code/world_gen/resources/arctic_skybox/back.jpg"};

    texture = loadCubemap(faces);
}

glm::mat4 SkyboxRenderer::getProjViewMatrix(Camera& camera)
{
    glm::mat4 proj = glm::perspective(glm::radians(camera.zoom), (float)settings.getWindowWidth() / (float)settings.getWindowHeight(), settings.zNear, settings.zFar);
    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
    return proj*view;
}

void SkyboxRenderer::setProjViewMatrix(glm::mat4& projView)
{
    skybox_shaderprogram.setUniformMat4("projView", projView);
}

void SkyboxRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/skybox.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/skybox.frag";
    const char * compute_path = nullptr;
    skybox_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
    skybox_shaderprogram.setUniformInt("skybox", 0);
}

