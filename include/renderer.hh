#ifndef RENDERER_HH
#define RENDERER_HH

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "texture.hh"
#include "shader.hh"
#include "card.hh"
#include "pile.hh"

class Renderer
{
public:
    // Constructor (inits shaders/shapes)
    Renderer(Shader shader);
    // Destructor
    ~Renderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawTexture(Texture texture);
    void DrawPile(Pile pile);
private:
    // Render state
    Shader shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif