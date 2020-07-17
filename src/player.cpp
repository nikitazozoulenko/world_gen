#include <player.h>

Player::Player(Camera& camera) :  camera(camera)
{

}


float Player::distanceTo(float x, float y, float z)
{
    float eps = 0.00000001;
    return std::sqrt((x-camera.pos.x)*(x-camera.pos.x) + (y-camera.pos.y)*(y-camera.pos.y) + (z-camera.pos.z)*(z-camera.pos.z) + eps);
}


float Player::distanceTo(glm::vec3 point)
{
    return distanceTo(point.x, point.y, point.z);
}