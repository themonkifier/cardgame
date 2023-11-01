#include "../include/card.hh"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

bool Card::operator==(const Card& c) const noexcept
{
    return this->objectInfo == c.objectInfo;
}