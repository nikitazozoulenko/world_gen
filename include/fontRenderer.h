#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>

#include <vector>
#include <map>


struct Character 
{
    unsigned int texID;    // ID handle of the glyph texture
    glm::ivec2 size;       // Size of glyph
    glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;  // Offset to advance to next glyph
};


class FontDrawer
{
public:
    FontDrawer(std::string truetype_file_path);
    void draw(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    std::map<char, Character> character_map;
    unsigned int VAO;
    unsigned int VBO;
    Shaderprogram char_shaderprogram;

    void makeCharacterTextureMap(std::string truetype_file_path);
    void setupVAOVBO();
    void setupCharShader();
};