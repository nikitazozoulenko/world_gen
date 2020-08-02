#include <chunk.h>
#include <misc.h>

Chunk::Chunk(Settings& settings, glm::ivec2 pos, unsigned int* blocks, int numRenderFaces[6], 
        std::unordered_map<glm::vec3, unsigned int, std::hash<glm::vec3>> renderFacesMap[6]) :
    settings(settings),
    pos(pos),
    p_block_model(new BlockModel()),
    first_vbo_init(true),
    re_init_vaos(true),
    blocks(blocks)
{
    for(int i=0; i<6; i++){
        num_render_faces[i] = numRenderFaces[i];
        render_faces_map[i] = renderFacesMap[i];
    }
}


Chunk::~Chunk()
{
    print_vec2("deleting chunk ", pos);
    if(blocks)
    {
        delete blocks;
    }
}

// Chunk::Chunk(const Chunk& source) : Chunk(source.settings, source.pos)
// {
//     print_vec2("copy", pos);
// } //copy TODO


// Chunk& Chunk::operator=(const Chunk& source)
// {
//     print_vec2("assign", pos);
// } //assign TODO
    
    
// Chunk& Chunk::operator=(Chunk&& source)
// {
//     print_vec2("move", pos);
// } // move TODO


unsigned int& Chunk::getBlock(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    return blocks[x+ch_width*y+ch_width*ch_height*z];
}
unsigned int& Chunk::getBlock(glm::ivec3 v)
{
    return getBlock(v.x, v.y, v.z);
}
unsigned int& Chunk::getBlock(glm::vec3 v)
{
    return getBlock(glm::ivec3(v));
}


void Chunk::setBlock(int x, int y, int z, unsigned int block)
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    blocks[x+ch_width*y+ch_width*ch_height*z] = block;
}
void Chunk::setBlock(glm::ivec3 v, unsigned int block)
{
    setBlock(v.x, v.y, v.z, block);
}
void Chunk::setBlock(glm::vec3 v, unsigned int block)
{
    setBlock(glm::ivec3(v), block);
}


bool Chunk::blockIsInChunk(int x, int y, int z) //local
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    if(x < ch_width  && x >= 0)
        if(y < ch_height && y >= 0)
            if(z < ch_depth && z >= 0)
                return true;
    return false;
}


void Chunk::visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID) //local coordinates
{   
    glm::vec3 p;
    if(face == BlockModel::EAST)
        p = glm::vec3(x+1, y, z);
    else if(face == BlockModel::WEST)
        p = glm::vec3(x-1, y, z);
    else if(face == BlockModel::TOP)
        p = glm::vec3(x, y+1, z);
    else if(face == BlockModel::BOTTOM)
        p = glm::vec3(x, y-1, z);
    else if(face == BlockModel::NORTH)
        p = glm::vec3(x, y, z+1);
    else if(face == BlockModel::SOUTH)
        p = glm::vec3(x, y, z-1);
    glm::vec3 global_pos = glm::vec3(pos.x*settings.getChunkWidth()+x, y, pos.y*settings.getChunkDepth()+z);

    removeFromRenderMap(face, global_pos);  //is this needed?
    if(blockIsInChunk(p.x, p.y, p.z))
    {   
        unsigned int block = getBlock(p);
        if(block == 0) //TODO TODO change to visible/transparant block
            addToRenderMap(blockID, face, global_pos);
    }
    else if(face == BlockModel::TOP && y == settings.getChunkHeight()-1)
    {
        addToRenderMap(blockID, BlockModel::TOP, global_pos);
    }

}


void Chunk::addToRenderMap(int blockID, int face, glm::vec3 pos)
{
    render_faces_map[face][pos] = blockID;
}


void Chunk::removeFromRenderMap(int face, glm::vec3 pos)
{
    render_faces_map[face].erase(pos);
}


void Chunk::rebuildVBOs(std::array<std::unordered_map<int, int>,6>& texArrayIDLookup) //every time it is needed when a chunk is to be drawn
{
    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {   
        //remove old vbos
        if(!first_vbo_init)
        {
            glDeleteBuffers(1, &p_block_model->pos_VBOs[face]);
            glDeleteBuffers(1, &p_block_model->texArrayID_VBOs[face]);
        }

        //gather data for new vbos
        int size = render_faces_map[face].size();
        num_render_faces[face] = size;
        glm::vec3 positions[size];
        int texArrayIDs[size];
        int counter = 0;
        for (auto& pair : render_faces_map[face])
        {
            glm::vec3 pos = pair.first;
            unsigned int block = pair.second;

            positions[counter] = pos;
            texArrayIDs[counter] = texArrayIDLookup[face][block];
            counter++;
        }

        //bind the data to vbos
        glBindVertexArray(p_block_model->VAOs[face]);

        glGenBuffers(1, &(p_block_model->pos_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->pos_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glGenBuffers(1, &(p_block_model->texArrayID_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->texArrayID_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), &texArrayIDs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
    }
    first_vbo_init = false;
}


void Chunk::render(std::array<std::unordered_map<int, int>,6>& texArrayIDLookup)
{
    if (re_init_vaos)
    {
        rebuildVBOs(texArrayIDLookup);
        re_init_vaos = false;
    }

    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {
        glBindVertexArray(p_block_model->VAOs[face]);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, num_render_faces[face]);
    }
}