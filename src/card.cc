#include "../include/card.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

const glm::mat4 Card::identity(1.0);

Card::Card(unsigned const int x, unsigned const int y, unsigned const int width, unsigned const int height, Shader* shader)
{
    this->shader = shader;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    translation = glm::mat4(1.0f);
    position = glm::vec2(0.0f, 0.0f);
    angle = 0;
}

void Card::show()
{
    translation = glm::translate(identity, glm::vec3(position.x, position.y, 0.0f));
    translation = glm::rotate(translation, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(translation));
}

void Card::rotate(float angle)
{
    this->angle = fmod(this->angle + angle, 2 * glm::pi<float>());
}


void Card::setRotation(float angle)
{
    this->angle = fmod(angle, 2 * glm::pi<float>());
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
{}