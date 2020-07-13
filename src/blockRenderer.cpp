#include <blockRenderer.h>
#include <misc.h>

BlockRenderer::BlockRenderer(Settings& settings) :
    settings(settings)
{
    createShaders();
    setup_block_texture();
}

void BlockRenderer::render(ChunkMapivec2& chunk_map, Camera& camera)
{
    //bind shaders
    block_shaderprogram.bind();

    //update view matrix every frame   (proj matrix really should NOT be set every frame)
    setProjectionMatrix(camera);
    setViewMatrix(camera);

    //render cubes
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture);
    for (auto& pair : chunk_map)
    {
        //TODO: frustrum culling?
        pair.second.render(texArrayIDLookup);
    }
}


void BlockRenderer::setProjectionMatrix(Camera& camera)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)settings.getWindowWidth() / (float)settings.getWindowHeight(), 0.1f, 1000.0f);
    block_shaderprogram.setUniformMat4("projection", projection);
}


void BlockRenderer::setViewMatrix(Camera& camera)
{
    glm::mat4 view = camera.getViewMatrix();
    block_shaderprogram.setUniformMat4("view", view);
}


void BlockRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/block.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/block.frag";
    const char * compute_path = nullptr;
    block_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
    block_shaderprogram.setUniformVec3("sun_dir", glm::vec3(1,2,3));
}


void BlockRenderer::setup_block_texture()
{
    int block_width = 512;
    int n_tex = 8;
    int n_blocks=7;
    std::string path = "/home/nikita/Code/world_gen/resources/blocks/wholeblocktextures.png";
    block_texture = loadTextureArray(path.c_str(), block_width, n_tex);

    setupTexArrayIDLookup(n_blocks);
}


void BlockRenderer::setupTexArrayIDLookup(int n_blocks)
{
    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {   
        int i = 0;
        for(int blockID=1; blockID<=n_blocks; blockID++)
        {   
            int temp_i = i;
            if(blockID == 2) //TODO change to hashtable for strings
            {
                if(face == BlockModel::TOP)
                    temp_i=2;
                else if(face == BlockModel::BOTTOM)
                    temp_i=0;
                else
                    temp_i=1;
                i++;
            }
            texArrayIDLookup[face][blockID] = temp_i;
            i++;
        }
    }
}