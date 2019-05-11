#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


// Include standard headers
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdlib>


#define HEIGHT 6
#define WIDTH 6
int MAX_LIGHT_VALUE = 2;
std::string world[WIDTH][HEIGHT];
int sunlight_level[WIDTH];


void create_world()
{
    for(int x=0; x<WIDTH; x++)
    {
        for(int y=0; y<HEIGHT; y++)
        {
            world[x][y] = "0";
        }
    }
    world[0][1] = "#";
    world[0][3] = "#";

    world[1][1] = "#";

    world[2][1] = "#";

    world[3][1] = "#";
    world[3][3] = "#";

    world[4][1] = "#";
    world[4][3] = "#";
}


void show_world()
{
    for (int y=HEIGHT-1; y>-1; y--)
    {
        for (int x=0; x<WIDTH; x++)
        {
            std::cout << world[x][y];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


bool is_block(int x, int y)
{
    if(world[x][y] == "#")
        return true;
    return false;
}


bool blockIsInChunk(int x, int y) //local
{
    if(x < WIDTH  && x >= 0)
        if(y < HEIGHT && y >= 0)
            return true;
    return false;
}


void light_BFS_helper_func(int light_value, int x, int y, std::unordered_set<glm::ivec2, std::hash<glm::ivec2>>& new_points)
{
    if(blockIsInChunk(x, y))
    {
        if(!is_block(x,y))
        {
            if (light_value > 1+std::atoi(world[x][y].c_str()))
            {
                world[x][y] = std::to_string(light_value-1);
                new_points.insert(glm::ivec2(x,y));
            }
        }
    }
}

void recursive_light_BFS(std::unordered_set<glm::ivec2, std::hash<glm::ivec2>>& points)
{
    //make one iteration at every point
    std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> new_points;
    for(auto& p: points)
    {
        int light_value = std::atoi(world[p.x][p.y].c_str());
        light_BFS_helper_func(light_value, p.x+1, p.y,   new_points);
        light_BFS_helper_func(light_value, p.x-1, p.y,   new_points);
        light_BFS_helper_func(light_value, p.x,   p.y+1, new_points);
        light_BFS_helper_func(light_value, p.x,   p.y-1, new_points);
    }

    //if new_points isnt empty, recurse
    if (!new_points.empty())
        recursive_light_BFS(new_points);
}



//sunlight_level is the lowest air block which sunlight. 
void find_start_points()
{
    //find sunlight level
    for (int x=0; x<WIDTH; x++)
    {
        sunlight_level[x] = 0;
        for (int y=HEIGHT-1; y>-1; y--)
        {
            if(is_block(x,y))
            {
                sunlight_level[x] = y+1;
                break;
            }
        }
    }


    //set first max sunlight values, then get the wanted points.
    std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> points;
    for (int x=0; x<WIDTH; x++)
        for (int y=HEIGHT-1; y>-1; y--)
        {
            //direct sunlight
            if(y>= sunlight_level[x])
            {
                world[x][y] = std::to_string(MAX_LIGHT_VALUE);
            }
            else
            {
                break;
            }

            //look for interesting BFS points and add to set
            bool interesting_point = false;
            if(x>0 && x<WIDTH-1)
            {
                if(sunlight_level[x-1]>y || sunlight_level[x+1]>y)
                    interesting_point = true;
            }
            else if(x==0)
            {
                if(sunlight_level[x+1]>y)
                    interesting_point = true;
            }
            else //if(x==WIDTH-1)
            {
                if(sunlight_level[x-1]>y)
                    interesting_point = true;
            }
            if (interesting_point)
                points.insert(glm::ivec2(x,y));
        }
    recursive_light_BFS(points);
}




int main()
{
    create_world();
    show_world();
    find_start_points();
    show_world();
    return 0;
}







