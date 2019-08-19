#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../include/misc.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

unsigned int loadTexture(char const * path)
{   
    // stbi_set_flip_vertically_on_load(true);
    std::cout << path << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;


        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


unsigned int loadTextureArray(char const * path, int img_width, int n_blocks)
{
    std::cout << path << std::endl;
    // stbi_set_flip_vertically_on_load(false);
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, img_width, img_width, n_blocks, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D_ARRAY,
                        GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,
                        GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        std::cout << "Texture array failed to load at path: " << path << std::endl;
    }
    std::cout << "tex8" << std::endl;
    stbi_image_free(data);
    std::cout << "tex9" << std::endl;
    return texture;
}


//----print functions for easier debugging
//-------------------------------------------------------
void print_vec3(std::string text, glm::vec3 vec)
{
    std::cout << text << " x:" << vec.x <<" y:" << vec.y << " z" << vec.z << std::endl;
}


void print_vec2(std::string text, glm::vec2 vec)
{
    std::cout << text << " x:" << vec.x <<" y:" << vec.y << std::endl;
}

void print_text(std::string text)
{
    std::cout << text << std::endl;
}


//#include <chrono>
// auto start = std::chrono::high_resolution_clock::now();
// auto stop = std::chrono::high_resolution_clock::now();
// auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
// std::cout << "Time taken by the tested function: "<< duration.count() << " microseconds" << std::endl; 


uint32_t wang_hash(uint32_t seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}