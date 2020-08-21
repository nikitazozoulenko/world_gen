#include <player.h>

Player::Player(Settings& settings, Camera& camera) :  settings(settings), camera(camera), inventory(settings.hotbar_size, 0)
{

}


float Player::distanceTo(float x, float y, float z)
{
    return distanceTo(glm::vec3(x,y,z));
}


float Player::distanceTo(glm::vec3 point)
{   
    return glm::distance(camera.pos, point);
}