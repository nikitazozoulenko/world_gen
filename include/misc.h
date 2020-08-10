#ifndef MY_MISC_FUNCS_H
#define MY_MISC_FUNCS_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path);
unsigned int loadTextureArray(std::unordered_map<std::string, unsigned int>& pathToIndexMap, int img_width);
unsigned int loadCubemap(std::vector<std::string> faces);
void print_vec4(std::string text, glm::vec4 vec);
void print_vec3(std::string text, glm::vec3 vec);
void print_vec2(std::string text, glm::vec2 vec);
void print_float(std::string text, float val);
void print_text(std::string text);

#endif // MY_MISC_FUNCS_H

