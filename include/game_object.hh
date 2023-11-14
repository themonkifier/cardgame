#ifndef GAME_OBJECT_HH
#define GAME_OBJECT_HH

#include <iostream>
#include <string>
#include <memory>

#include "shader.hh"
#include "texture.hh"
#include "resource_manager.hh"

class GameObject
{
public:
    struct ObjectInfo
    {
        std::string variation;
        std::pair<std::string, int> value;

        bool operator==(const ObjectInfo& oi) const noexcept;
        ObjectInfo(std::string suit, std::pair<std::string, int> value);
        ObjectInfo();
        friend std::ostream& operator<<(std::ostream& os, const GameObject::ObjectInfo& oi) {
            os << oi.value.first << " of " << oi.variation << " (" << oi.value.second << ")";
            return os;
        }
    } objectInfo;

    Texture texture;

    void flipOver();

    GameObject(ObjectInfo objectInfo, bool alpha);
    bool operator==(const GameObject& c) const noexcept;
    bool operator<(const GameObject& c) const noexcept;
    std::size_t operator()(const GameObject& gameobject) const noexcept;
};

template<> struct std::hash<GameObject>
{
    std::size_t operator()(const GameObject& gameobject) const noexcept
    {
        return 10665624317 * std::hash<std::string>{}(gameobject.objectInfo.variation) % 46995126991 + 86166321749;
    }
};

#endif