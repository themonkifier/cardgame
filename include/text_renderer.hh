#ifndef TEXT_RENDERER_HH
#define TEXT_RENDERER_HH

#include <unordered_map>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hh"

class TextRenderer {
public:
    TextRenderer(Shader &shader);
    void DrawText(std::string text, float x, float y, float scale = 1, glm::vec3 color = {1.0f, 1.0f, 1.0f});

private:
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::unordered_map<GLchar, Character> Characters;
    unsigned int VAO, VBO;

    Shader& shader;
};

#endif