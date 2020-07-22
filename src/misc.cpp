#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <misc.h>

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


unsigned int loadTextureArray(std::unordered_map<std::string, unsigned int>& pathToIndexMap, int img_width)
{
    // stbi_set_flip_vertically_on_load(false);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                0,                // level
                GL_RGBA8,         // Internal format
                img_width, img_width, pathToIndexMap.size(), // width,height,depth
                0,                // border?
                GL_RGBA,          // format
                GL_UNSIGNED_BYTE, // type
                0);               // pointer to data

    for(auto& pair : pathToIndexMap){
        auto& path = pair.first;
        auto& index = pair.second;
        int width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            if(nrComponents != 4){
                print_float(path, nrComponents);
                throw "format must be GL_RGBA, ie 4 components";
            }
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                            0,                     //Mipmap number
                            0,0,index,                 //xoffset, yoffset, zoffset
                            img_width,img_width,1, //width, height, depth
                            GL_RGBA,                //format
                            GL_UNSIGNED_BYTE,      //type
                            data);                 //pointer to data
        }
        else
        {
            std::cout << "Texture array failed to load at path: " << path << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                    GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY); //maybe this at the end? not for every subImage3D

    return texture;
}


//----print functions for easier debugging
//-------------------------------------------------------

void print_vec4(std::string text, glm::vec4 vec)
{
    std::cout << text << " x:" << vec.x <<" y:" << vec.y << " z:" << vec.z << " w:" << vec.w << std::endl;
}


void print_vec3(std::string text, glm::vec3 vec)
{
    std::cout << text << " x:" << vec.x <<" y:" << vec.y << " z:" << vec.z << std::endl;
}


void print_vec2(std::string text, glm::vec2 vec)
{
    std::cout << text << " x:" << vec.x <<" y:" << vec.y << std::endl;
}


void print_float(std::string text, float val)
{
    std::cout << text << val << std::endl;
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