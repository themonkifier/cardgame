#include "../include/renderer.hh"


Renderer::Renderer(Shader shader)
    : shader(shader)
{
    initRenderData();
}

Renderer::~Renderer()
{
    if (this != nullptr) {
        glDeleteVertexArrays(1, &VAO);
    }
}

void Renderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    // render textured quad
    this->shader.SetVector3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawTexture(Texture texture)
{

    // bounds checking!
    // if (texture.position.x < 0) texture.position.x = 0;
    // else if (texture.position.x > SCREEN_WIDTH - texture.size.x) texture.position.x = SCREEN_WIDTH - texture.size.x;

    // if (texture.position.y < 0) texture.position.y = 0;
    // else if (texture.position.x > SCREEN_WIDTH - texture.size.x) texture.position.x = SCREEN_WIDTH - texture.size.x;

    // prepare transformations
    shader.Use();
    texture.updateTranslation();

    shader.SetMatrix4("model", texture.translation);

    // render textured quad
    shader.SetVector3("spriteColor", texture.tint);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawGameObject(GameObject gameobject) {
    DrawTexture(gameobject.texture);
}

void Renderer::initRenderData()
{
    // configure VAO/VBO
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}