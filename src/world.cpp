#include <world.h>
#include <misc.h>
#include <mymath.h>

#include <algorithm>

World::World(Settings& settings):
    settings(settings),
    chunk_manager(ChunkManager(settings, chunk_map))
{

}



ChunkManager::ChunkManager(Settings& settings, ChunkMapivec2& chunk_map):
    settings(settings),
    chunk_map(chunk_map),
    size(60),
    amplitude(20)
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
    glBindTexture(GL_TEXTURE_2D, comp_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, ch_width, ch_depth, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, comp_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}


void ChunkManager::createComputeShader()
{   
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    const char * compute_path =  "/home/nikita/Code/world_gen/src/shaders/generation.comp";
    comp_shaderprogram = Shaderprogram(nullptr, nullptr, nullptr, compute_path, {{"MAX_OCTAVES","const int MAX_OCTAVES="+std::to_string(settings.MAX_OCTAVES)}});
    comp_shaderprogram.bind();
    comp_shaderprogram.setUniformInt("CH_WIDTH", ch_width);
    comp_shaderprogram.setUniformInt("CH_HEIGHT", ch_height);
    comp_shaderprogram.setUniformInt("CH_DEPTH", ch_depth);
    comp_shaderprogram.setUniformFloat("game_time", glfwGetTime());
}


void ChunkManager::createChunk(glm::ivec2 pos)
{
    removeChunk(pos);
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    comp_shaderprogram.bind();
    comp_shaderprogram.setUniformVec2("chunk_pos", pos);
    comp_shaderprogram.setUniformFloat("size", size);
    comp_shaderprogram.setUniformFloat("amplitude", amplitude);

    //run compute shader
    glBindTexture(GL_TEXTURE_2D, comp_texture);
    glDispatchCompute(ch_width, 1, ch_depth);
    //wait for the compute shader to be done before reading textures
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    //construct in-place
    chunk_map.emplace(std::piecewise_construct,
                      std::forward_as_tuple(pos),
                      std::forward_as_tuple(settings, pos));

    //update visibility on edge
    updateEdges(pos);
}


void ChunkManager::removeChunk(glm::ivec2 pos)
{
    if (chunk_map.count(pos) > 0)
        chunk_map.erase(pos);
}

void ChunkManager::gen_new_nearby_chunks(glm::vec3& center_pos) //in world coords
{
    float& x = center_pos.x;
    float& y = center_pos.y;
    float& z = center_pos.z;
    int x_ch = std::floor(x / (float)settings.getChunkWidth());
    int y_ch = std::floor(y / (float)settings.getChunkHeight());
    int z_ch = std::floor(z / (float)settings.getChunkDepth());
    // given R, make a square with sidelength 2R. then filter out dist. then sort dist
    int R = settings.getRenderDistance();
    glm::ivec2 player_pos_ch = glm::ivec2(x_ch, z_ch);
    std::vector<glm::ivec2> chunk_positions;
    for(int x_r=-R; x_r<=R; x_r++)
    {
        for(int z_r=-R; z_r<=R; z_r++)
        {
            glm::ivec2 coord = glm::ivec2(x_r, z_r);
            if (R > glm::length(glm::vec2(coord)))
            {
                chunk_positions.push_back(coord);
            }
        }
    } //now sort based on distance from origin
    std::sort(chunk_positions.begin(), chunk_positions.end(), dist_from_origin);
    for(glm::ivec2& coord : chunk_positions)
    {
        glm::ivec2 pos = player_pos_ch + coord;
        if(chunk_map.count(pos) == 0){
            createChunk(pos);
            break; //only one per frame
        }
    }
}


void ChunkManager::remove_far_chunks(glm::vec3& center_pos) //in world coords
{
    float& x = center_pos.x;
    float& y = center_pos.y;
    float& z = center_pos.z;
    int x_ch = std::floor(x / (float)settings.getChunkWidth());
    int y_ch = std::floor(y / (float)settings.getChunkHeight());
    int z_ch = std::floor(z / (float)settings.getChunkDepth());
    // given R, make a square with sidelength 2R. then filter out dist. then sort dist
    int R = settings.getRenderDistance();
    glm::ivec2 player_pos_ch = glm::ivec2(x_ch, z_ch);
    for(auto& pair : chunk_map)
    {
        glm::ivec2 pos = pair.first;
        if(glm::length(glm::vec2(pos-player_pos_ch))>=R)
        {
            removeChunk(pos);
            break; //only one per frame, also for loop is broken by removing an element from chunkmap
        }
    }
}

void ChunkManager::updateEdges(glm::ivec2& pos)
{
    glm::ivec2 pos_east = glm::ivec2(pos.x+1, pos.y);
    glm::ivec2 pos_west = glm::ivec2(pos.x-1, pos.y);
    glm::ivec2 pos_north = glm::ivec2(pos.x, pos.y+1);
    glm::ivec2 pos_south = glm::ivec2(pos.x, pos.y-1);

    if(chunk_map.count(pos_west) != 0)
        updateBlockVisEdge(chunk_map.at(pos_west), BlockModel::WEST, chunk_map.at(pos), BlockModel::EAST);
    if(chunk_map.count(pos_east) != 0)
        updateBlockVisEdge(chunk_map.at(pos), BlockModel::WEST, chunk_map.at(pos_east), BlockModel::EAST);
    if(chunk_map.count(pos_south) != 0)
        updateBlockVisEdge(chunk_map.at(pos_south), BlockModel::SOUTH, chunk_map.at(pos), BlockModel::NORTH);
    if(chunk_map.count(pos_north) != 0)
        updateBlockVisEdge(chunk_map.at(pos), BlockModel::SOUTH, chunk_map.at(pos_north), BlockModel::NORTH);
}


void ChunkManager::updateBlockVisEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2)
{   
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();
    glm::vec3 side;
    glm::vec3 k;
    int W;
    if(face2==BlockModel::EAST)
    {
        side = glm::vec3(0,0,1);
        k = glm::vec3(1,0,0);
        W = ch_depth;
    }
    else
    {
        side = glm::vec3(1,0,0);
        k = glm::vec3(0,0,1);
        W = ch_width;
    }
    
    glm::vec3 corner = glm::vec3(chunk2.pos.x*ch_width, 0, chunk2.pos.y*ch_depth);
    for(int y=0; y<ch_height; y++)
        for(int i=0; i<W; i++)
        {
            glm::vec3 pos1 = glm::vec3(corner + glm::vec3(0,y,0) + glm::vec3(side.x*i, 0, side.z*i));
            glm::vec3 pos2 = pos1-k;
            unsigned int& block1 = getBlockGlobal(pos1);
            unsigned int& block2 = getBlockGlobal(pos2);

            if(block1==0 && block2!=0)
            {
                chunk1.addToRenderMap(block2, face2, pos2);
                chunk1.re_init_vaos = true;
            }

            if(block1!=0 && block2==0)
            {
                chunk2.addToRenderMap(block1, face1, pos1);
                chunk2.re_init_vaos = true;
            }
        }
}


unsigned int& ChunkManager::getBlockGlobal(glm::vec3 p)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    //WARNING check if in bounds outside of this function
    int chunk_x = std::floor(p.x / (float)ch_width);
    int chunk_z = std::floor(p.z / (float)ch_depth);
    int local_x = p.x - chunk_x*ch_width;
    int local_y = p.y;
    int local_z = p.z - chunk_z*ch_depth;
    unsigned int& block = chunk_map.at(glm::ivec2(chunk_x, chunk_z)).getBlock(local_x, local_y, local_z);
    return block;
}
