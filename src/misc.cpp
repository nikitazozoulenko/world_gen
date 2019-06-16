#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../include/misc.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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