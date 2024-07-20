#include "../include/resource_manager.hh"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/stb_image.h"

std::unordered_map<std::string, Texture> ResourceManager::Textures;
std::unordered_map<std::string, Shader>  ResourceManager::Shaders;


Shader& ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    Shaders[name] = Shader(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture& ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
    Textures[name] = _loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders
    for (auto it : Shaders)
        glDeleteProgram(it.second.ID);
    // (properly) delete all textures
    for (auto it : Textures)
        glDeleteTextures(1, &it.second.ID);
}

Texture ResourceManager::_loadTextureFromFile(const char *file, bool alpha)
{
    // create texture object
    Texture texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}