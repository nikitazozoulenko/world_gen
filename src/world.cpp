#include <world.h>
#include <misc.h>
#include <mymath.h>

#include <algorithm>

World::World(Settings& settings, Camera& camera):
    settings(settings),
    chunk_manager(ChunkManager(settings, chunk_map)),
    block_is_targeted(false),
    player(camera)
{

}


void World::gameLogic(double delta_time)
{
    glm::vec3& player_pos = player.camera.pos;
    chunk_manager.remove_far_chunks(player_pos);
    chunk_manager.gen_new_nearby_chunks(player_pos);
    targetBlockRay(player_pos);
}


double rayProjectionHelperFun(double& x, double& dir)
{
    double len = x - std::floor(x);
    if(dir>0){
        if(len==0)
            len=1;
        else
            len = 1-len;
    }
    else{//dir_x>0 or 0, but then we return inf and thats fine, since all dir_xyz wont be 0
        if(len==0)
            len=-1;
        else
            len = -len;
    }
    return len/dir;
}
void World::targetBlockRay(glm::vec3 v, glm::vec3 enter_direction, int step){targetBlockRay(v.x, v.y, v.z, enter_direction, step);}
void World::targetBlockRay(double x, double y, double z, glm::vec3 enter_direction, int step) //warning for stack overflow segfault
{
    block_is_targeted = false;
    double dir_x = player.camera.front.x;
    double dir_y = player.camera.front.y;
    double dir_z = player.camera.front.z;
    if(player.distanceTo(x,y,z) < settings.reach)
    {
        if(step==0){
            target_pos = glm::vec3(std::floor(x), std::floor(y), std::floor(z));
        }
        else{
            target_pos += enter_direction;
        }
        if(isInBounds(target_pos))
        {
            unsigned int block = getBlock(target_pos);
            if(block != 0)
            {
                block_is_targeted = true;
                target_facing = -enter_direction;
                return;
            }
        }

        //projection on cube sides
        double tx = rayProjectionHelperFun(x, dir_x);
        double ty = rayProjectionHelperFun(y, dir_y);
        double tz = rayProjectionHelperFun(z, dir_z);
        if(tx<ty && tx<tz)
            targetBlockRay(std::nearbyint(x+tx*dir_x), y+tx*dir_y, z+tx*dir_z, glm::vec3(sign(dir_x),0,0), step+1);
        else if(ty<tx && ty<tz)
            targetBlockRay(x+ty*dir_x, std::nearbyint(y+ty*dir_y), z+ty*dir_z, glm::vec3(0,sign(dir_y),0), step+1);
        else
            targetBlockRay(x+tz*dir_x, y+tz*dir_y, std::nearbyint(z+tz*dir_z), glm::vec3(0,0,sign(dir_z)), step+1);
    }
}


void World::placeBlockOnCursor(unsigned int block)
{
    if(block_is_targeted){
        setBlock(target_pos+target_facing, block);
    }
}
void World::destroyBlockOnCursor()
{
    if(block_is_targeted){
        setBlock(target_pos, 0);
    }
}


unsigned int& World::getBlock(glm::vec3 p){return chunk_manager.getBlock(p.x, p.y, p.z);}
unsigned int& World::getBlock(int x, int y, int z){return chunk_manager.getBlock(x, y, z);}
bool World::isInBounds(glm::vec3 p){return chunk_manager.isInBounds(p);}
bool World::isInBounds(int x, int y, int z){return chunk_manager.isInBounds(x, y, z);}
void World::setBlock(glm::vec3 v, int blockID){return chunk_manager.setBlock(v, blockID);}
void World::setBlock(int x, int y, int z, int blockID){return chunk_manager.setBlock(x,y,z, blockID);}


////////////////////////////////////
///////////CHUNKMANAGER/////////////
////////////////////////////////////


ChunkManager::ChunkManager(Settings& settings, ChunkMapivec2& chunk_map):
    settings(settings),
    chunk_map(chunk_map),
    size(60),
    amplitude(20)
{
    createComputeTexture();
    createComputeShader();
}


void ChunkManager::createComputeTexture()
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
    //removeChunk(pos);
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
    float& z = center_pos.z;
    int x_ch = std::floor(x / (float)settings.getChunkWidth());
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
            unsigned int& block1 = getBlock(pos1);
            unsigned int& block2 = getBlock(pos2);

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


std::pair<glm::ivec2, glm::vec3> ChunkManager::findChunkPos(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int chunk_x = std::floor(x / (float)ch_width);
    int chunk_z = std::floor(z / (float)ch_depth);
    int local_x = x - chunk_x*ch_width;
    int local_z = z - chunk_z*ch_depth;
    return std::pair<glm::ivec2, glm::vec3>(glm::ivec2(chunk_x, chunk_z), glm::vec3(local_x,y,local_z));
}
std::pair<glm::ivec2, glm::vec3> ChunkManager::findChunkPos(glm::vec3 v)
{
    return findChunkPos(v.x, v.y, v.z);
}


unsigned int& ChunkManager::getBlock(int x, int y, int z)
{
    glm::ivec2 chunk_pos;
    glm::vec3 local_pos;
    std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);
    unsigned int& block = chunk_map.at(chunk_pos).getBlock(local_pos);
    return block;
}
unsigned int& ChunkManager::getBlock(glm::vec3 p)
{
    return getBlock(p.x, p.y, p.z);
}


bool ChunkManager::isInBounds(glm::vec3 v)
{
    return isInBounds(v.x, v.y, v.z);
}
bool ChunkManager::isInBounds(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();

    int chunk_x = std::floor(x / (float)ch_width);
    int chunk_z = std::floor(z / (float) ch_depth);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<ch_height)
        if(chunk_map.find(pos)!=chunk_map.end())
            return true;
    return false;
}


void ChunkManager::setBlock(glm::vec3 v, int blockID)
{
    setBlock(v.x, v.y, v.z, blockID);
}
void ChunkManager::setBlock(int x, int y, int z, int blockID)
{
    glm::ivec2 chunk_pos;
    glm::vec3 local_pos;
    std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);

    chunk_map.at(chunk_pos).setBlock(local_pos, blockID);
    updateVisible(x, y, z); //100 ish
    updateVisible(x+1, y, z);
    updateVisible(x-1, y, z);
    updateVisible(x, y+1, z);
    updateVisible(x, y-1, z);
    updateVisible(x, y, z+1);
    updateVisible(x, y, z-1);
}

void ChunkManager::updateVisible(int x, int y, int z)
{
    //adds sides of block at positions x y z if needed. first removes all faces then adds if needed. do this for all 7 blocks.
    if(isInBounds(x, y, z))
    {
        unsigned int first_block = getBlock(x,y,z);
        glm::vec3 first_block_pos = glm::vec3(x,y,z);
        //loop over 6 sides
        std::unordered_map<int, glm::vec3> positions = {{BlockModel::EAST,  glm::vec3(x+1, y,   z  )},
                                                        {BlockModel::WEST,  glm::vec3(x-1, y,   z  )},
                                                        {BlockModel::TOP,   glm::vec3(x  , y+1, z  )},
                                                        {BlockModel::BOTTOM,glm::vec3(x  , y-1, z  )},
                                                        {BlockModel::NORTH, glm::vec3(x  , y,   z+1)},
                                                        {BlockModel::SOUTH, glm::vec3(x  , y,   z-1)}};
        int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
        glm::ivec2 chunk_pos;
        glm::vec3 local_pos;
        std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);
        for(int& face : faces)
        {
            //remove current face
            Chunk& chunk = chunk_map.at(chunk_pos);
            chunk.removeFromRenderMap(face, first_block_pos);
            chunk.re_init_vaos = true;

            glm::vec3 p = positions[face];
            if(isInBounds(p))
            {
                unsigned int block = getBlock(p);
                if(block == 0 && first_block != 0)  //TODO change to visible/nonvisible blocks?? (fence)
                {
                    //add face if checked block isnt air
                    chunk.addToRenderMap(first_block, face, first_block_pos);
                }
            }
            else if(p.y == settings.getChunkHeight() && first_block != 0)
            {
                chunk.addToRenderMap(first_block, face, first_block_pos);  //TODO change lighting
            }
        }
    }
}
