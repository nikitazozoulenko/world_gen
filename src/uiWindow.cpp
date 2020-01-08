#include <uiWindow.h>
#include <algorithm>

UIWindow::UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color):
    coords(coords),
    width(width),
    height(height),
    color(color),
    time_last_press(glfwGetTime()),
    held_down(false)
{
    setup();
}


void UIWindow::setup()
{
    float vertices[] = {
        //pos         //tex coords
        -1, -1, 0.0f, 0.0f, 0.0f,
         1, -1, 0.0f, 1.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1, -1, 0.0f, 0.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1,  1, 0.0f, 0.0f, 1.0f
    };

        // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // // load textures 
    // texture = loadTexture(("/home/nikita/Code/world_gen/resources/"+filename+".png").c_str());
}


UIWindow* UIWindow::find_uiwindow_on_cursor(std::vector<UIWindow>& ui_windows, float& x, float& y)
{   
    //find all ui_windows that are under the cursor to later pick the top most one.
    std::vector<int> window_indices;
    for(int i=0; i<ui_windows.size(); i++){   
        UIWindow& ui_window = ui_windows[i];
        float& x0 = ui_window.coords.x;
        float& y0 = ui_window.coords.y;
        float& width = ui_window.width;
        float& height = ui_window.height;

        //if (x,y) in window
        if(x<(x0+width) && x0<x){
            if(y<(y0+height) && y0<y){
                window_indices.push_back(i);
            }
        }
    }

    //now pick the top most window
    if(window_indices.size()>0){
        std::sort(window_indices.begin(), window_indices.end(), [&ui_windows](const auto& lhs, const auto& rhs){return ui_windows[lhs].time_last_press > ui_windows[rhs].time_last_press;});
        return &ui_windows[window_indices[0]];
    }
    //if none found, return nullptr
    return nullptr;
}

void UIWindow::uiwindow_mouse_move_callback(int& mouse_state, std::vector<UIWindow>& ui_windows, float& xoffset, float& yoffset, float& x, float& y)
{
    //if mouse pressed (state is either PRESS or RELEASE)
    if (mouse_state == GLFW_PRESS)
    {
        UIWindow* p_pressed_uiwindow = find_uiwindow_on_cursor(ui_windows, x, y);
        if (p_pressed_uiwindow)
        {
            bool& held_down = p_pressed_uiwindow->held_down;
            float& x0 = p_pressed_uiwindow->coords.x;
            float& y0 = p_pressed_uiwindow->coords.y;
            if(held_down)
            {//MOUSE HOLD LOGIC
                x0 += xoffset;
                y0 += yoffset;
            }
        }
    }
}

void UIWindow::uiwindow_click_callback(int& mouse_state, std::vector<UIWindow>& ui_windows, float& x,float& y)
{
    if (mouse_state == GLFW_PRESS)
    {
        UIWindow* p_pressed_uiwindow = find_uiwindow_on_cursor(ui_windows, x, y);
        if (p_pressed_uiwindow)
        {
            p_pressed_uiwindow->held_down=true;
            p_pressed_uiwindow->time_last_press = glfwGetTime();
        }
    }
    else//if mouse_state == GLFW_RELEASE
    {
        for (auto& uiwindow : ui_windows)
        {//reset all was pressed last callback for next callback
            uiwindow.held_down = false;
        }
    }
    
}