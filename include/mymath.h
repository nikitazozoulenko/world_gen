#ifndef MY_OWN_MATH_FUNCTIONS_H
#define MY_OWN_MATH_FUNCTIONS_H

#include <iostream>

#include <glm/glm.hpp>
#include <misc.h>

bool dist_from_origin(const glm::ivec2& lhs, const glm::ivec2& rhs);
double sign(double x);
float sign(float x);
int sign(int x);

#endif // MY_OWN_MATH_FUNCTIONS_H

