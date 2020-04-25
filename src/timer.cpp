#include <timer.h>
#include <iostream>

Timer::Timer():
    current_time(0)
{

}

// for FPS counter.   NOTE: glfwPollEvents has built in 60 fps limit.
double Timer::update_delta_time()
{
    float currentFrame = glfwGetTime();
    delta_time = currentFrame - current_time;
    current_time = currentFrame;
    std::cout << 1/delta_time << std::endl;
    return delta_time;
}