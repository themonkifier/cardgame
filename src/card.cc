#include "../include/card.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Card::Card(const char* imagePath, Shader* shader)
{
    this->shader = shader;

    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    identity = glm::mat4(1.0f);
    translation = glm::mat4(1.0f);
    position = glm::vec2(0.0f, 0.0f);
    angle = 0;
}

void Card::show()
{
    translation = glm::translate(identity, glm::vec3(position.x, position.y, 0.0f));
    translation = glm::rotate(identity, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(translation));
    
}

void Card::rotate(float angle)
{
    angle += angle;
}

void Card::translate(glm::vec2 positionDifference)
{
    position += positionDifference;
}

void Card::move(glm::vec2 newPosition)
{
    position = newPosition;
}

Card::~Card()
{
    glDeleteTextures(1, &texture);
}