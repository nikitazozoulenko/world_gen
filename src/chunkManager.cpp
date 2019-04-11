#include "../include/chunkManager.h"
#include "../include/misc.h"

ChunkManager::ChunkManager()
{
    this->stay_alive = true;
    // TEMP TEMP TEMP TEMP

    for(int x=-1; x<4; x++)
    {
        for (int z=-0; z<4; z++)
        {
            glm::ivec2 chunk_pos = glm::ivec2(x, z);
            addChunk(chunk_pos, world_gen.generateChunk(chunk_pos));
        }
    }
}


void ChunkManager::startMainThread()
{
    //TODO
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
    int chunk_x = std::floor(x / (float)Chunk::WIDTH);
    int chunk_z = std::floor(z / (float) Chunk::BREADTH);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<Chunk::HEIGHT)
        if(chunk_map.find(pos)!=chunk_map.end())
            return true;
    return false;
}


void ChunkManager::updateVisible(float x, float y, float z, int offset)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    updateVisible(x_int, y_int, z_int, offset);
}


void ChunkManager::updateVisible(int x, int y, int z, int offset)
{
    //TODO : WARNING : (maybe -- probably) doesnt work properly on the corner of a chunk when the nearby chunk is unloaded
    if(isInBounds(x, y, z))
    {
        //first block lighting here for render_info
        BlockInfo first_block_info = getBlockInfo(x,y,z);

        //loop over 6 sides
        int positions[6][3] = {{x+offset, y,        z       },
                               {x-offset, y,        z       },
                               {x,        y+offset, z       },
                               {x,        y-offset, z       },
                               {x,        y,        z+offset},
                               {x,        y,        z-offset}};
        int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
        int ch_x = std::floor(x / (float)Chunk::WIDTH);
        int ch_z = std::floor(z / (float)Chunk::BREADTH);
        for(int& face : faces)
        {
            glm::vec3 p = glm::vec3(positions[face][0], positions[face][1], positions[face][2]);
            if(isInBounds(p.x, p.y, p.z))
            {
                //remove current face
                Chunk& chunk = chunk_map[glm::ivec2(ch_x, ch_z)];
                chunk.removeFromRenderMap(face, p);
                chunk.re_init_vaos = true;

                BlockInfo& block_info = getBlockInfo(p.x, p.y, p.z);
                if(block_info.blockID != 0 && first_block_info.blockID == 0)  //change to visible/nonvisible blocks?? (fence)
                {
                    //add face if checked block isnt air
                    chunk.addToRenderMap(block_info.blockID, face, first_block_info.lighting, p);
                }
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
        int ch_x = std::floor(x / (float)Chunk::WIDTH);
        int ch_z = std::floor(z / (float)Chunk::BREADTH);

        //REMOVE BLOCK
        info.blockID=blockID;

        //UPDATE LIGHTING
        //updateLightingHereSomethingVeryLongFunction();

        //last param is that you have to check the reverse sides for the block surrounding the changed block
        updateVisible(x, y, z, -1);
        updateVisible(x+1, y, z, -1);
        updateVisible(x-1, y, z, -1);
        updateVisible(x, y+1, z, -1);
        updateVisible(x, y-1, z, -1);
        updateVisible(x, y, z+1, -1);
        updateVisible(x, y, z-1, -1);
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
    int chunk_x = std::floor(x / (float)Chunk::WIDTH);
    int chunk_z = std::floor(z / (float)Chunk::BREADTH);
    int local_x = x - chunk_x*Chunk::WIDTH;
    int local_y = y;
    int local_z = z - chunk_z*Chunk::BREADTH;
    BlockInfo& info = chunk_map[glm::ivec2(chunk_x, chunk_z)].getBlockInfo(local_x, local_y, local_z);
    return info;
}


void ChunkManager::addChunk(glm::ivec2 pos, Chunk chunk)
{   
    //add chunk
    chunk_map[pos] = chunk;

    //update lighting
    //updateLighting(all the chunks affected);

    //update visibility on edge
    if(chunk_map.find(glm::ivec2(pos.x-1, pos.y))!=chunk_map.end())
        updateVisChunkEdge(chunk_map[glm::ivec2(pos.x-1, pos.y)], BlockModel::WEST, chunk_map[pos], BlockModel::EAST);
    if(chunk_map.find(glm::ivec2(pos.x, pos.y-1))!=chunk_map.end())
        updateVisChunkEdge(chunk_map[glm::ivec2(pos.x, pos.y-1)], BlockModel::SOUTH, chunk_map[pos], BlockModel::NORTH);
}


void ChunkManager::updateVisChunkEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2)
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
    
    glm::vec3 corner = glm::vec3(chunk2.position.x*Chunk::WIDTH, 0, chunk2.position.y*Chunk::BREADTH);
    for(int y=0; y<Chunk::HEIGHT; y++)
        for(int i=0; i<Chunk::WIDTH; i++)
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

