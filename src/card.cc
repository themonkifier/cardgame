#include "../include/card.hh"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Card::Card(ObjectInfo objectInfo, bool alpha, Texture& back)
         : GameObject(objectInfo, alpha), back(back)
{}

bool Card::operator==(const Card& c) const noexcept
{
    return objectInfo == c.objectInfo && texture.zlevel == c.texture.zlevel;
}


void Card::flipOver()
{
    unsigned tempID = texture.ID;
    texture.ID = back.ID;
    back.ID = tempID;
    /*
    Texture temp = std::move(texture);

    texture = std::move(back);
    back = std::move(temp);
    texture.size = temp.size;
    texture.position = temp.size;
    texture.position = temp.position;
    texture.lastClickPos = temp.lastClickPos;
    texture.tint = temp.tint;
    texture.angle = temp.angle;
    texture.translation = temp.translation;
    texture.zlevel = temp.zlevel;
    */
}