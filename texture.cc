#include <iostream>
#include <memory>

#include "../include/texture.hh"

unsigned int Texture::globalzlevel = 0;

const glm::mat4 Texture::identity(1.0);

Texture::Texture()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT),
      Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR), tint(glm::vec3(1.0f, 1.0f, 1.0f)),
      size(glm::vec2(140.0f, 190.0f)), position(glm::vec2(0.0f, 0.0f)), angle(0),
      translation(glm::mat4(1.0f)), isClicked(false), isDraggable(true), zlevel(globalzlevel++)
{
    glGenTextures(1, &this->ID);
}

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->Width = width;
    this->Height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::updateTranslation()
{
    translation = glm::translate(identity, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    translation = glm::translate(translation, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    translation = glm::rotate(translation, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    translation = glm::translate(translation, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    translation = glm::scale(translation, glm::vec3(size, 1.0f)); // last scale
}

void Texture::rotate(float angle)
{
    this->angle = fmod(this->angle + angle, 2 * glm::pi<float>());
}


void Texture::setRotation(float angle)
{
    this->angle = fmod(angle, 2 * glm::pi<float>());
}

void Texture::translate(glm::vec2 positionDifference)
{
    position += positionDifference;
}

void Texture::move(glm::vec2 newPosition)
{
    position = newPosition;
}

void Texture::onClick(glm::vec2 position)
{
    isClicked = true;
    zlevel = globalzlevel++;
    // TODO: update draw order based on level, make zlevels not just mindlessly increase?
}

void Texture::onHold(glm::vec2 position)
{
    translate(position);
}

std::size_t Texture::operator()(const Texture& t) const noexcept
{
    return 64625999413 * t.Height % 71757757307 + 28167578773;
}

bool Texture::contains(glm::vec2 position, bool drag)
{
    unsigned int CLOSE_ENOUGH = drag ? CLOSE_ENOUGH_DRAG : CLOSE_ENOUGH_CLICK;
    return (position.x + CLOSE_ENOUGH >= this->position.x && position.x <= this->position.x + this->size.x)
     && (position.y + CLOSE_ENOUGH >= this->position.y && position.y <= this->position.y + this->size.y);
}

bool Texture::contains(std::pair<double, double> position, bool drag)
{
    unsigned int CLOSE_ENOUGH = drag ? CLOSE_ENOUGH_DRAG : CLOSE_ENOUGH_CLICK;

    return (position.first + CLOSE_ENOUGH >= this->position.x && position.first <= this->position.x + this->size.x)
     && (position.second + CLOSE_ENOUGH >= this->position.y && position.second <= this->position.y + this->size.y);
}

bool Texture::contains(std::pair<double, double> position)
{
    return (position.first >= this->position.x && position.first <= this->position.x + this->size.x)
     && (position.second >= this->position.y && position.second <= this->position.y + this->size.y);
}