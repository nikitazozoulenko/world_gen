#include "../include/player.h"

Player::Player(glm::vec3 pos) : pos(pos), camera(Camera(this->pos))
{
    this->reach = 7;
}


float Player::distanceTo(float x, float y, float z)
{
    float eps = 0.00000001;
    return std::sqrt((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) + eps);
}


float Player::distanceTo(glm::vec3 point)
{
    return distanceTo(point.x, point.y, point.z);
}