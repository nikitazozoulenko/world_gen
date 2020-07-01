#include <mymath.h>

bool dist_from_origin(const glm::ivec2& lhs, const glm::ivec2& rhs)
{
    return glm::length(glm::vec2(lhs)) < glm::length(glm::vec2(rhs));
}