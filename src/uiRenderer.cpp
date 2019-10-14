#include "../include/uiRenderer.h"

#include <iostream>
#include "../include/misc.h"

#include <ft2build.h>
#include FT_FREETYPE_H  


FontDrawer::FontDrawer(std::string truetype_file_path)
{
    makeCharacterTextureMap(truetype_file_path);
    setupVAOVBO();
    setupCharShader();
}


void FontDrawer::makeCharacterTextureMap(std::string truetype_file_path)
{
    //load library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    //load font
    FT_Face face;
    if (FT_New_Face(ft, truetype_file_path.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    //font size
    FT_Set_Pixel_Sizes(face, 0, 48);

    //Store each character in std::map
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    for (unsigned int c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int) face->glyph->advance.x
        };
        character_map.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Fix changed byte-alignment restriction
}


void FontDrawer::draw(std::string text, float x, float y, float scale, glm::vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Activate corresponding render state	
    char_shaderprogram.bind();
    char_shaderprogram.setUniformVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = character_map[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}


void FontDrawer::setupVAOVBO()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void FontDrawer::setupCharShader()
{
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/text_char.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/text_char.frag";
    const char * compute_path =  nullptr;

    char_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
    char_shaderprogram.bind();
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
    char_shaderprogram.setUniformMat4("projection", projection);
}


UIRenderer::UIRenderer():
    fontDrawer(FontDrawer("/home/nikita/Code/world_gen/resources/KratosTrueType.ttf"))
{
    createShaders();
    createUI();
}


void UIRenderer::render()
{
    //bind shaders
    ui_shaderprogram.bind();

    //render here
    for (auto& quad : quads)
    {
        quad.draw(ui_shaderprogram);
    }
    fontDrawer.draw("My Test", 25.0f, 25.0f, 1.0f, glm::vec3(1,1,1));
}


void UIRenderer::createShaders()
{
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/ui.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/ui.frag";
    const char * compute_path =  nullptr;

    ui_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
}


void UIRenderer::createUI()
{
    float size = 0.1f;
    quads.push_back(UIQuad(-size/16, size/16, -size/9, size/9, "wallstone_height"));
}


UIQuad::UIQuad(float x0, float x1, float y0, float y1, std::string filename)
{
    setup(x0, x1, y0, y1, filename);
}


void UIQuad::setup(float x0, float x1, float y0, float y1, std::string filename)
{
    float vertices[] = {
        //pos         //tex coords
        x0, y0, 0.0f, 0.0f, 0.0f,
        x1, y0, 0.0f, 1.0f, 0.0f,
        x1, y1, 0.0f, 1.0f, 1.0f,
        x0, y0, 0.0f, 0.0f, 0.0f,
        x1, y1, 0.0f, 1.0f, 1.0f,
        x0, y1, 0.0f, 0.0f, 1.0f
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

    // load textures 
    texture = loadTexture(("/home/nikita/Code/world_gen/resources/"+filename+".png").c_str());
}


void UIQuad::draw(Shaderprogram& shaderprogram)
{
    // bind vao, texture
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
