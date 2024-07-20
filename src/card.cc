#include "../include/card.hh"
#include "../include/resource_manager.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Card::Card(Suit suit, std::pair<char, int> value, bool alpha, Texture back)
         : back(back), value(value), alpha(alpha)
{
    const std::string TEXTURE_PATH = "../cards/", FILE_FORMAT = alpha ? ".png" : ".jpg";
    std::string name;
    name += static_cast<char>(suit);
    name += value.first;
    Texture texture = ResourceManager::LoadTexture((TEXTURE_PATH + name + FILE_FORMAT).c_str(), alpha, name);

    this->texture = texture;
}

bool Card::operator==(const Card& c) const noexcept
{
    return value == c.value && texture.zlevel == c.texture.zlevel;
}


void Card::flipOver()
{
    if (back) {
        unsigned tempID = texture.ID;
        texture.ID = back.value().ID;
        back.value().ID = tempID;
    }
}

std::string Card::toSocketRepresentation() {
    std::string ret;

    ret += static_cast<char>(suit);
    ret += value.first;

    return ret;
}

Card Card::fromSocketRepresentation(std::string socket_representation) {
    Suit suit = static_cast<Suit>(socket_representation[0]);

    std::pair<char, int> value;
    switch (socket_representation[1]) {
        case 'A':
        value = std::pair<char, int>('A', 1);
        break;

        case '2':
        value = std::pair<char, int>('2', 2);
        break;

        case '3':
        value = std::pair<char, int>('3', 3);
        break;

        case '4':
        value = std::pair<char, int>('4', 4);
        break;

        case '5':
        value = std::pair<char, int>('5', 5);
        break;

        case '6':
        value = std::pair<char, int>('6', 6);
        break;

        case '7':
        value = std::pair<char, int>('7', 7);
        break;

        case '8':
        value = std::pair<char, int>('8', 8);
        break;

        case '9':
        value = std::pair<char, int>('9', 9);
        break;

        case 'T':
        value = std::pair<char, int>('T', 10);
        break;

        case 'J':
        value = std::pair<char, int>('J', 11);
        break;

        case 'Q':
        value = std::pair<char, int>('Q', 12);
        break;

        case 'K':
        value = std::pair<char, int>('K', 13);
        break;
    }

    return {suit, value};
}

bool Card::operator<(const Card& c) const noexcept
{
    return texture.zlevel < c.texture.zlevel;
}

std::size_t Card::operator()(const Card& c) const noexcept {
    return 10665624317 * std::hash<char>{}(static_cast<char>(c.suit)) % 46995126991 + 86166321749;
}
