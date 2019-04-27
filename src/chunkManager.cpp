#include "../include/chunkManager.h"
#include "../include/misc.h"

#include <thread>
#include <cmath>
#include <algorithm>


void createAndAddChunkThread(glm::ivec2 ch_pos, ChunkManager* p_chunk_manager)
{
    //code here
    p_chunk_manager->addChunk(ch_pos);

    //at the end
    std::lock_guard<std::mutex> lock(p_chunk_manager->ch_being_generated_set_mutex);
    p_chunk_manager->ch_being_generated_set.erase(ch_pos);
}


bool dist_from_origin(const glm::ivec2& lhs, const glm::ivec2& rhs)
{
    return glm::length(glm::vec2(lhs)) < glm::length(glm::vec2(rhs));
}


void removeChunksOutOfRange(ChunkManager* p_chunk_manager, int R, glm::ivec2 player_ch_pos)
{
    std::lock_guard<std::mutex> lock(p_chunk_manager->ch_map_mutex);
    auto& chunk_map = p_chunk_manager->chunk_map;
    std::vector<glm::ivec2> remove_pos;
    for (auto& pair : chunk_map)
    {
        glm::ivec2 pos = pair.first;
        if (R <= glm::length(glm::vec2(pos-player_ch_pos)))
        {
            if(chunk_map.count(pos) != 0)
            {
                remove_pos.push_back(pos);
            }
        }
    }
    for (auto& pos : remove_pos)
        chunk_map.erase(pos);
}


void chunkMainThreadFunction(ChunkManager* p_chunk_manager)
{   
    int R = 5;
    while(p_chunk_manager->stay_alive)
    {
        // given R, make a square with sidelength 2R. then filter out dist. then sort dist
        int player_ch_x = std::floor(p_chunk_manager->player.pos.x / CH_WIDTH);
        int player_ch_z = std::floor(p_chunk_manager->player.pos.z / CH_WIDTH);
        glm::ivec2 player_ch_pos = glm::ivec2(player_ch_x, player_ch_z);
        std::vector<glm::ivec2> chunk_positions; // player is origin
        for(int ch_x=-R; ch_x<=R; ch_x++)
        {
            for(int ch_z=-R; ch_z<=R; ch_z++)
            {
                glm::ivec2 coord = glm::ivec2(ch_x, ch_z);
                if (R > glm::length(glm::vec2(coord)))
                {
                    chunk_positions.push_back(coord);
                }
            }
        }

        //now sort the coordinates
        std::sort(chunk_positions.begin(), chunk_positions.end(), dist_from_origin);

        //and use the closest chunks
        std::lock_guard<std::mutex> lock(p_chunk_manager->ch_being_generated_set_mutex);
        auto& set = p_chunk_manager->ch_being_generated_set;
        for(glm::ivec2& coord : chunk_positions)
        {
            glm::ivec2 ch_pos = coord+player_ch_pos;
            if(set.size() < p_chunk_manager->n_workers)
            {
                if(set.count(ch_pos) == 0)
                {
                    std::lock_guard<std::mutex> lock(p_chunk_manager->ch_map_mutex);
                    if(p_chunk_manager->chunk_map.count(ch_pos)==0)
                    {
                        set.insert(ch_pos);
                        std::thread t(createAndAddChunkThread, ch_pos, p_chunk_manager);
                        t.detach();
                    }
                }
            }
        }

        removeChunksOutOfRange(p_chunk_manager, R, player_ch_pos);
    }
}


ChunkManager::ChunkManager(Player& player) : 
    player(player),
    stay_alive(true),
    n_workers(4)
{

}


void ChunkManager::startMainThread()
{
    stay_alive = true;
    std::thread t(chunkMainThreadFunction, this);
    t.detach();
}


std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>>& ChunkManager::getChunkMap()
{
    return chunk_map;
}


std::mutex& ChunkManager::getMutex()
{
    return ch_map_mutex;
}


void ChunkManager::endMainThread()
{
    //TODO
}


bool ChunkManager::isInBounds(float x, float y, float z)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    return isInBounds(x_int, y_int, z_int);
}


bool ChunkManager::isInBounds(int x, int y, int z)
{
    int chunk_x = std::floor(x / (float)CH_WIDTH);
    int chunk_z = std::floor(z / (float) CH_WIDTH);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<CH_HEIGHT)
        if(chunk_map.find(pos)!=chunk_map.end())
            return true;
    return false;
}


void ChunkManager::updateVisible(float x, float y, float z)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    updateVisible(x_int, y_int, z_int);
}


void ChunkManager::updateVisible(int x, int y, int z)
{
    //adds sides of block at positions x y z if needed. first removes all faces then adds if needed. do this for all 7 blocks.
    if(isInBounds(x, y, z))
    {
        BlockInfo first_block_info = getBlockInfo(x,y,z);
        glm::vec3 first_block_pos = glm::vec3(x,y,z);
        //loop over 6 sides
        std::unordered_map<int, std::vector<int>> positions = { {BlockModel::EAST,  {x+1, y,   z  }},
                                                                {BlockModel::WEST,  {x-1, y,   z  }},
                                                                {BlockModel::TOP,   {x  , y+1, z  }},
                                                                {BlockModel::BOTTOM,{x  , y-1, z  }},
                                                                {BlockModel::NORTH, {x  , y,   z+1}},
                                                                {BlockModel::SOUTH, {x  , y,   z-1}} };
        int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
        int ch_x = std::floor(x / (float)CH_WIDTH);
        int ch_z = std::floor(z / (float)CH_WIDTH);
        for(int& face : faces)
        {
            //remove current face
            Chunk& chunk = chunk_map[glm::ivec2(ch_x, ch_z)];
            chunk.removeFromRenderMap(face, first_block_pos);
            chunk.re_init_vaos = true;

            glm::vec3 p = glm::vec3(positions[face][0], positions[face][1], positions[face][2]);
            if(isInBounds(p.x, p.y, p.z))
            {
                BlockInfo& block_info = getBlockInfo(p.x, p.y, p.z);
                if(block_info.blockID == 0 && first_block_info.blockID != 0)  //TODO change to visible/nonvisible blocks?? (fence)
                {
                    //add face if checked block isnt air
                    chunk.addToRenderMap(first_block_info.blockID, face, block_info.lighting, first_block_pos);
                }
            }
            else if(p.y == CH_HEIGHT && first_block_info.blockID != 0)
            {
                chunk.addToRenderMap(first_block_info.blockID, face, 1.0f, first_block_pos);  //TODO change lighting
            }
        }
    }
}


void ChunkManager::changeBlock(float x, float y, float z, int blockID)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    changeBlock(x_int, y_int, z_int, blockID);
}


void ChunkManager::changeBlock(int x, int y, int z, int blockID)
{
    if (isInBounds(x, y, z))
    {
        BlockInfo& info = getBlockInfo(x, y, z);
        int ch_x = std::floor(x / (float)CH_WIDTH);
        int ch_z = std::floor(z / (float)CH_WIDTH);

        //REMOVE BLOCK
        info.blockID=blockID;

        //UPDATE LIGHTING
        //updateLightingHereSomethingVeryLongFunction();

        //last param is that you have to check the reverse sides for the block surrounding the changed block
        updateVisible(x, y, z);
        updateVisible(x+1, y, z);
        updateVisible(x-1, y, z);
        updateVisible(x, y+1, z);
        updateVisible(x, y-1, z);
        updateVisible(x, y, z+1);
        updateVisible(x, y, z-1);
    }
}


BlockInfo& ChunkManager::getBlockInfo(float x, float y, float z)
{
    //WARNING check if in bounds outside of this function
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    return getBlockInfo(x_int,y_int,z_int);
}


BlockInfo& ChunkManager::getBlockInfo(int x, int y, int z)
{
    //WARNING check if in bounds outside of this function
    int chunk_x = std::floor(x / (float)CH_WIDTH);
    int chunk_z = std::floor(z / (float)CH_WIDTH);
    int local_x = x - chunk_x*CH_WIDTH;
    int local_y = y;
    int local_z = z - chunk_z*CH_WIDTH;
    BlockInfo& info = chunk_map[glm::ivec2(chunk_x, chunk_z)].getBlockInfo(local_x, local_y, local_z);
    return info;
}


void ChunkManager::addChunk(glm::ivec2 pos)
{   
    //add chunk
    Chunk chunk = world_gen.generateChunk(pos);

    std::lock_guard<std::mutex> lock(ch_map_mutex);
    chunk_map[pos] = chunk;

    //update lighting (does this need to be locked?)
    //updateLighting(all the chunks affected);

    // //update visibility on edge
    updateEdges(pos);
}


void ChunkManager::updateEdges(glm::ivec2& pos)
{
    glm::ivec2 pos_east = glm::ivec2(pos.x+1, pos.y);
    glm::ivec2 pos_west = glm::ivec2(pos.x-1, pos.y);
    glm::ivec2 pos_north = glm::ivec2(pos.x, pos.y+1);
    glm::ivec2 pos_south = glm::ivec2(pos.x, pos.y-1);

    if(chunk_map.count(pos_west) != 0)
        updateBlockVisEdge(chunk_map[pos_west], BlockModel::WEST, chunk_map[pos], BlockModel::EAST);
    if(chunk_map.count(pos_east) != 0)
        updateBlockVisEdge(chunk_map[pos], BlockModel::WEST, chunk_map[pos_east], BlockModel::EAST);
    if(chunk_map.count(pos_south) != 0)
        updateBlockVisEdge(chunk_map[pos_south], BlockModel::SOUTH, chunk_map[pos], BlockModel::NORTH);
    if(chunk_map.count(pos_north) != 0)
        updateBlockVisEdge(chunk_map[pos], BlockModel::SOUTH, chunk_map[pos_north], BlockModel::NORTH);
}


void ChunkManager::updateBlockVisEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2)
{   
    glm::vec3 side;
    glm::vec3 k;
    if(face2==BlockModel::EAST)
    {
        side = glm::vec3(0,0,1);
        k = glm::vec3(1,0,0);
    }
    else
    {
        side = glm::vec3(1,0,0);
        k = glm::vec3(0,0,1);
    }
    
    glm::vec3 corner = glm::vec3(chunk2.position.x*CH_WIDTH, 0, chunk2.position.y*CH_WIDTH);
    for(int y=0; y<CH_HEIGHT; y++)
        for(int i=0; i<CH_WIDTH; i++)
        {
            glm::vec3 pos1 = glm::vec3(corner + glm::vec3(0,y,0) + glm::vec3(side.x*i, 0, side.z*i));
            glm::vec3 pos2 = pos1-k;
            BlockInfo info1 = getBlockInfo(pos1.x, pos1.y, pos1.z);
            BlockInfo info2 = getBlockInfo(pos2.x, pos2.y, pos2.z);

            if(info1.blockID==0 && info2.blockID!=0)
            {
                chunk2.addToRenderMap(info2.blockID, face2, info1.lighting, pos2);
                chunk2.re_init_vaos = true;
            }

            //is working
            if(info1.blockID!=0 && info2.blockID==0)
            {
                chunk1.addToRenderMap(info1.blockID, face1, info2.lighting, pos1);
                chunk1.re_init_vaos = true;
            }
        }
}

