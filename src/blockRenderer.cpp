#include <blockRenderer.h>
#include <misc.h>

BlockRenderer::BlockRenderer(Settings& settings) :
    settings(settings)
{
    createShaders();
}


void BlockRenderer::render(ChunkMapivec2& chunk_map, Camera& camera)
{
    //uniforms for vertex and fragment shader, marching cube
    march_cube_draw_shaderprogram.bind();
    march_cube_draw_shaderprogram.setUniformFloat("game_time", glfwGetTime());
    glm::mat4 view = camera.getViewMatrix();
    march_cube_draw_shaderprogram.setUniformMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)settings.getWindowWidth() / (float)settings.getWindowHeight(), 0.1f, 1000.0f);
    march_cube_draw_shaderprogram.setUniformMat4("projection", projection);

    //render
    march_cube_draw_shaderprogram.bind();
    for (auto& pair : chunk_map)
    {
        glm::ivec2 pos = pair.first;
        Chunk& chunk = pair.second;
        chunk.render();
    }
}


void BlockRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/marchCubeDraw.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/marchCubeDraw.frag";
    
    march_cube_draw_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, nullptr);
}