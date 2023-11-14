#ifndef TEXT_HH
#define TEXT_HH

#include <string>
#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Font {
public:
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    FT_Library ft;
    FT_Face face;
    std::map<char, Character> Characters;

    Font(const char* fontname = "arial", unsigned const int height = 48);
};

#endif