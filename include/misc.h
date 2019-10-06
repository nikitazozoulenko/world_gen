#ifndef MY_MISC_FUNCS_H
#define MY_MISC_FUNCS_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path);
unsigned int loadTextureArray(char const * path, int img_width, int n_blocks);
void print_vec4(std::string text, glm::vec4 vec);
void print_vec3(std::string text, glm::vec3 vec);
void print_vec2(std::string text, glm::vec2 vec);
void print_float(std::string text, float val);
void print_text(std::string text);

#endif // MY_MISC_FUNCS_H

