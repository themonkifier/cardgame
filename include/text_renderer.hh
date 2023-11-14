#ifndef TEXT_RENDERER_HH
#define TEXT_RENDERER_HH

#include "shader.hh"
#include "font.hh"
#include "info.hh"

class TextRenderer {
public:
    unsigned int VAO, VBO;
    TextRenderer(Shader s, Font f);

    void DrawText(std::string text, float x, float y, float scale = 1, glm::vec3 color = {1.0f, 1.0f, 1.0f});
private:
    Shader shader;
    Font font;
};


#endif