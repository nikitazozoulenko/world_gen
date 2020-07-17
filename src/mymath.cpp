#include <mymath.h>

bool dist_from_origin(const glm::ivec2& lhs, const glm::ivec2& rhs)
{
    return glm::length(glm::vec2(lhs)) < glm::length(glm::vec2(rhs));
}

double sign(double x)
{
    return (x > 0) - (x < 0);
}
float sign(float x)
{
    return (x > 0) - (x < 0);
}
int sign(int x)
{
    return (x > 0) - (x < 0);
}