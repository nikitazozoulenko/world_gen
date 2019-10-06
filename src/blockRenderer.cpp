#include "../include/blockRenderer.h"
#include "../include/misc.h"

BlockRenderer::BlockRenderer(Camera* p_camera) :
    p_camera(p_camera)
{
    createShaders();
    createMarchComputeTexture();
}


void BlockRenderer::createMarchComputeTexture()
{
    glGenTextures(1, &comp_texture);
    glBindTexture(GL_TEXTURE_3D, comp_texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, CH_WIDTH+1, CH_HEIGHT+1, CH_DEPTH+1, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, comp_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    //////////////////////////////////////////////////////////////
    // Work group sizes
    // MAX GLOBAL
    int work_grp_cnt[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
    printf("max global (total) work group size x:%i y:%i z:%i\n", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
    // MAX LOCAL
    int work_grp_size[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n", work_grp_size[0], work_grp_size[1], work_grp_size[2]);
    // MAX LOCAL INVOCATIONS
    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    printf("max local work group invocations %i\n", work_grp_inv);
}


void BlockRenderer::render()
{
    //uniforms for vertex and fragment shader, marching cube
    march_cube_draw_shaderprogram.bind();
    march_cube_draw_shaderprogram.setUniformFloat("game_time", glfwGetTime());
    glm::mat4 view = p_camera->getViewMatrix();
    march_cube_draw_shaderprogram.setUniformMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(p_camera->zoom), (float)1280 / (float)720, 0.1f, 1000.0f);
    march_cube_draw_shaderprogram.setUniformMat4("projection", projection);

    //uniforms compute shader terrain generation
    comp_shaderprogram.bind();
    comp_shaderprogram.setUniformFloat("game_time", glfwGetTime());
    comp_shaderprogram.setUniformInt("CH_WIDTH", CH_WIDTH);
    comp_shaderprogram.setUniformInt("CH_HEIGHT", CH_HEIGHT);
    comp_shaderprogram.setUniformInt("CH_DEPTH", CH_DEPTH);
    comp_shaderprogram.setUniformVec2("chunk_pos", glm::vec2(0,0));

    //compute shader
    glDispatchCompute(CH_WIDTH+1, CH_HEIGHT+1, CH_DEPTH+1);
    //wait for the compute shader to be done before binding the comp texture
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    Chunk chunk(glm::ivec2(0,0));

    //render
    march_cube_draw_shaderprogram.bind();
    chunk.render();
}


void BlockRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/marchCubeDraw.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/marchCubeDraw.frag";
    const char * compute_path =  "/home/nikita/Code/world_gen/src/shaders/generation.comp";

    march_cube_draw_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, nullptr);
    comp_shaderprogram = Shaderprogram(nullptr, nullptr, nullptr, compute_path);
}