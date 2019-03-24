#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include "camera.h"

#include <glm/glm.hpp>

class Player
{
public:
    Player(glm::vec3 pos=glm::vec3(0,0,0));
    int reach; // in blocks
    glm::vec3 pos;
    Camera camera;

    void jump();
    float distanceTo(float x, float y, float z);
    float distanceTo(glm::vec3 point);
private:
};

#endif // MY_PLAYER_H