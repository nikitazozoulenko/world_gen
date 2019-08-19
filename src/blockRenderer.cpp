#include "../include/blockRenderer.h"
#include "../include/misc.h"

BlockRenderer::BlockRenderer(Camera* p_camera) :
    p_camera(p_camera)
{
    setupQuad();
    createShaders();
    setupTextures();

    // dimensions of the image
    int WIDTH = 512;
    int HEIGHT = 512;
    glGenTextures(1, &ray_texture);
    glBindTexture(GL_TEXTURE_2D, ray_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, ray_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

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


void BlockRenderer::setupTextures()
{
    // Block textures
    block_normal = loadTexture("/home/nikita/Code/world_gen/resources/wallstone_normal.jpg");
    block_texture = loadTexture("/home/nikita/Code/world_gen/resources/wallstone_texture.jpg");
    std::cout << "BLOCK " << block_texture << " " << block_normal << std::endl;
}


void BlockRenderer::setupQuad()
{
    float vertices[] = 
    {   // positions // texture coords
         1,  1,  0,  1.0f, 1.0f,   // top right
         1, -1,  0,  1.0f, 0.0f,   // bottom right
        -1, -1,  0,  0.0f, 0.0f,   // bottom left
        -1,  1,  0,  0.0f, 1.0f    // top left 
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}


void BlockRenderer::render()
{
    unsigned int WIDTH = 512;
    unsigned int HEIGHT = 512;
    unsigned int SPP = 16; //note: have to edit this in compute shader code also

    ray_shaderprogram.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, block_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, block_normal);

    //uniforms
    ray_shaderprogram.setUniformVec3("cam_pos", p_camera->pos);
    ray_shaderprogram.setUniformVec3("cam_dir", p_camera->front);
    ray_shaderprogram.setUniformVec3("cam_up", p_camera->up);
    ray_shaderprogram.setUniformFloat("game_time", glfwGetTime());
    glDispatchCompute(WIDTH, HEIGHT, SPP);

    glBindVertexArray(quadVAO);
    quad_shaderprogram.bind();

    //wait for the compute shader to be done before binding the ray texture
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ray_texture);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void BlockRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/quad.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/quad.frag";
    const char * compute_path =  "/home/nikita/Code/world_gen/src/shaders/ray.comp";

    quad_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, nullptr);
    ray_shaderprogram = Shaderprogram(nullptr, nullptr, nullptr, compute_path);
    
    // sampler2D for additional textures
    ray_shaderprogram.bind();
    ray_shaderprogram.setUniformInt("texture_map", GL_TEXTURE1);
    ray_shaderprogram.setUniformInt("normal_map", GL_TEXTURE2);
    ray_shaderprogram.unbind();
}