#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include standard headers
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

glm::vec2 gradients[4] = {glm::vec2(-1.0, 1.0), glm::vec2(1.0, 1.0), glm::vec2(-1.0, 1.0), glm::vec2(-1.0, -1.0)};


int random(glm::ivec2 pos) //TODO integrate seed
{
    srand((((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0))*23));
    return rand() % 4;
}


float noise(float x, float y) //world x and y
{
    float noise = 0.0f;

    //scewed coordinates 
    float NNN = 2.0f;
    float F = (std::sqrt(NNN+1)-1)/NNN;
    float G = (1-1/std::sqrt(NNN+1))/NNN;
    float x_scew = x + (x+y)*F;
    float y_scew = y + (x+y)*F;

    //which hypercube coordinates 
    float x_b = std::floor(x_scew);
    float y_b = std::floor(y_scew);

    //internal coordinates
    float x_i = x_scew-x_b;
    float y_i = y_scew-y_b;

    //order for edge traversal
    glm::ivec2 corner = glm::ivec2(x_b, y_b);
    std::vector<glm::ivec2> corners = {corner};
    if(x_i > y_i)
        corners.push_back(corner + glm::ivec2(1, 0));
    else
        corners.push_back(corner + glm::ivec2(0, 1));
    corners.push_back(corner + glm::ivec2(1, 1));

    //traverse every edge
    for (glm::ivec2& vert : corners)
    {   
        float factor = (vert.x+vert.y)*G;
        float x_n = x-(vert.x - factor);
        float y_n = y-(vert.y - factor);

        //std::cout << "x " << x_n << " y " << y_n << std::endl; 

        float max = std::max(0.0f, 0.5f - x_n*x_n - y_n*y_n);
        //std::cout << max << std::endl;
        noise += max*max*max*max * glm::dot(gradients[random(vert)], glm::vec2(x_n, y_n));
    }
    //std::cout << noise << std::endl;

    return (70.0f * noise +1.0f)/2.0f;
}

int main()
{
    for(float x=0; x<80; x++)
    {
        for(float y=0; y<80; y++)
        {
            float x0 = x/80.0f;
            float y0 = y/80.0f;
            std::cout << std::floor(10*noise(x0, y0));
        }
        std::cout << std::endl;
    }

    return 0;
}







