#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include <camera.h>
#include <settings.h>

#include <glm/glm.hpp>

#include <vector>

class Player
{
public:
    Player(Settings& settings, Camera& camera);
    Camera& camera;

    std::vector<unsigned int> inventory;
    int selected_slot; //index

    void jump();
    float distanceTo(float x, float y, float z);
    float distanceTo(glm::vec3 point);
private:
    Settings& settings;
};

#endif // MY_PLAYER_H