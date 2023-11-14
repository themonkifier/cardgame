#include "../include/card.hh"

GameObject::GameObject(ObjectInfo objectInfo, bool alpha)
    : texture(texture), objectInfo(objectInfo)
{
    const std::string TEXTURE_PATH = "../cards/", FILE_FORMAT = alpha ? ".png" : ".jpg";
    std::string name;
    name += objectInfo.variation + objectInfo.value.first;
    Texture texture = ResourceManager::LoadTexture((TEXTURE_PATH + name + FILE_FORMAT).c_str(), alpha, name);

    this->texture = texture;
}

bool GameObject::operator==(const GameObject& c) const noexcept
{
    return this->objectInfo == c.objectInfo;
}

bool GameObject::operator<(const GameObject& c) const noexcept
{
    return texture.zlevel < c.texture.zlevel;
}

bool GameObject::ObjectInfo::operator==(const Card::ObjectInfo& oi) const noexcept
{
    return this->variation == oi.variation && this->value == oi.value;
}

GameObject::ObjectInfo::ObjectInfo(std::string variation, std::pair<std::string, int> value)
    : variation(variation), value(value)
{}

GameObject::ObjectInfo::ObjectInfo()
    : variation(""), value()
{}

std::size_t GameObject::operator()(const GameObject& gameobject) const noexcept
{
    return 10665624317 * std::hash<std::string>{}(gameobject.objectInfo.variation) % 46995126991 + 86166321749;
}

void GameObject::flipOver()
{
    std::cout << "why is GameObject::flipOver() being called?" << std::endl;
}