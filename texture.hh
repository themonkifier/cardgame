#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID;
    unsigned int Width, Height;
    unsigned int Internal_Format;
    unsigned int Image_Format;
    unsigned int Wrap_S, Wrap_T;
    unsigned int Filter_Min, Filter_Max;

    Texture();
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;
};

#endif