#ifndef MY_OWN_PROCGEN_H
#define MY_OWN_PROCGEN_H

#include <glm/glm.hpp>
#include <misc.h>

#include <vector>

unsigned int wang_hash(unsigned int seed);
unsigned int randUIntAccumulate(unsigned int& rand_state);
unsigned int ivec2ToUIntPRNG(glm::ivec2 v);
unsigned int ivec3ToUIntPRNG(glm::ivec3 v);
float randFloatAccumulate(unsigned int& rand_state);
float simplexNoise2D(float x, float y, float& amplitude, unsigned int& count);
float simplexNoise3D(float x, float y, float z, float& amplitude, unsigned int& count);
float simplexOctaves2D(float world_x, float world_y, unsigned int& count, std::vector<float>& sizes, std::vector<float>& amplitudes);




#endif // MY_OWN_PROCGEN_H

