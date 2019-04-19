#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include standard headers
#include <iostream>
#include <unordered_map>

#include "../include/worldGenerator.h"
#include "../include/block.h"
#include "../include/displaywindow.h"


int main()
{
    Displaywindow displaywindow = Displaywindow(1600, 900, "My Window"); //note, have to change SCRWIDTH SCRHEIGHT in multiple places (masterrenderer proj matrix)
    BlockModel bm;
    return 0;
}







