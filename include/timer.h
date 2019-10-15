#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Timer
{
public:
    Timer();
    double update_delta_time();
private:
    double delta_time;
    float current_time;
};

#endif // MY_TIMER_H