#include "../include/card.hh"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

const glm::mat4 Card::identity(1.0);

Card::Card(Texture texture, CardInfo cardInfo, glm::vec3 tint)
    : texture(texture), cardInfo(cardInfo), tint(tint), size(glm::vec2(140.0f, 190.0f)),
      position(glm::vec2(0.0f, 0.0f)), angle(0), translation(glm::mat4(1.0f)), isClicked(false), isDraggable(true)
{}

Card::Card(CardInfo cardInfo, bool alpha, glm::vec3 tint)
    : texture(texture), cardInfo(cardInfo), tint(tint), size(glm::vec2(140.0f, 190.0f)),
      position(glm::vec2(0.0f, 0.0f)), angle(0), translation(glm::mat4(1.0f)), isClicked(false), isDraggable(true)
{
    Texture texture;
    const std::string TEXTURE_PATH = "../cards/", FILE_FORMAT = alpha ? ".png" : ".jpg";
    std::string name;
    name += cardInfo.suit + cardInfo.rank;
    texture = ResourceManager::LoadTexture((TEXTURE_PATH + name + FILE_FORMAT).c_str(), alpha, name);

    this->texture = texture;
}

bool Card::operator==(const Card& c) const noexcept
{
    return this->cardInfo == c.cardInfo;
}

bool Card::CardInfo::operator==(const Card::CardInfo& ci) const noexcept
{
    return this->suit == ci.suit && this->rank == ci.rank && this->value == ci.value;
}


Card::CardInfo::CardInfo(std::string suit, std::string rank, int value)
    : suit(suit), rank(rank), value(value)
{}

Card::CardInfo::CardInfo()
    : suit(""), rank(""), value(0)
{}

void Card::updateTranslation()
{
    translation = glm::translate(identity, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    translation = glm::translate(translation, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    translation = glm::rotate(translation, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    translation = glm::translate(translation, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    translation = glm::scale(translation, glm::vec3(size, 1.0f)); // last scale
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

void Card::onClick(glm::vec2 position)
{
    isClicked = true;
    std::cout << "card clicked\n";
}

void Card::onHold(glm::vec2 position)
{
    translate(position);
    std::cout << "card held\n";
}