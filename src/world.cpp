#include <world.h>
#include <misc.h>

World::World(Settings& settings):
    settings(settings),
    chunk_manager(ChunkManager(settings, chunk_map))
{

}



ChunkManager::ChunkManager(Settings& settings, ChunkMapivec2& chunk_map):
    settings(settings),
    chunk_map(chunk_map),
    size(20),
    amplitude(1)
{
    createMarchComputeTexture();
    createComputeShader();

    for(int i=0; i<1; i++)
    {
        for(int j=0; j<1; j++)
        {
            createChunk(glm::ivec2(i,j));
        }
    }
}


void ChunkManager::createMarchComputeTexture()
{   
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    glGenTextures(1, &comp_texture);
    glBindTexture(GL_TEXTURE_3D, comp_texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, ch_width+1, ch_height, ch_depth+1, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, comp_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}


void ChunkManager::createComputeShader()
{   
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    const char * compute_path =  "/home/nikita/Code/world_gen/src/shaders/generation.comp";
    comp_shaderprogram = Shaderprogram(nullptr, nullptr, nullptr, compute_path);
    comp_shaderprogram.bind();
    comp_shaderprogram.setUniformInt("CH_WIDTH", ch_width);
    comp_shaderprogram.setUniformInt("CH_HEIGHT", ch_height);
    comp_shaderprogram.setUniformInt("CH_DEPTH", ch_depth);
    comp_shaderprogram.setUniformFloat("game_time", glfwGetTime());
}


void ChunkManager::createChunk(glm::ivec2 pos)
{
    if (chunk_map.count(pos) > 0)
        chunk_map.erase(pos);
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    comp_shaderprogram.bind();
    comp_shaderprogram.setUniformVec2("chunk_pos", pos);
    comp_shaderprogram.setUniformFloat("size", size);
    comp_shaderprogram.setUniformFloat("amplitude", amplitude);


    //run compute shader
    glDispatchCompute(ch_width+1, ch_height, ch_depth+1);
    //wait for the compute shader to be done before reading textures
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    Chunk chunk = Chunk(settings, pos);
    chunk_map.emplace(pos, chunk);
}
