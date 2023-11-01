#ifndef CARD_HH
#define CARD_HH

#include <string>
#include <memory>

#include "shader.hh"
#include "texture.hh"
#include "resource_manager.hh"
#include "game_object.hh"

class Card : public GameObject
{
public:
    // Card(ObjectInfo cardInfo, bool alpha, glm::vec3 tint = glm::vec3(1.0f, 1.0f, 1.0f));
    bool operator==(const Card& c) const noexcept;
};

#endif