#ifndef SHADER_HH
#define SHADER_HH

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
class Shader
{
public:
    // state
    unsigned int ID;
    // constructor
    Shader(const char * const vertexPath, const char * const fragmentPath, const char * const geometryPath = nullptr); // note: geometry source code is optional
    Shader() = default;

    std::size_t operator()(const Shader& s) const noexcept;

    // sets the current shader as active
    Shader& Use();
    // compiles the shader from given source code
    // utility functions
    void SetFloat  (const char * const name, const float value, const bool useShader = false);
    void SetInteger(const char * const name, const int value, const bool useShader = false);
    void SetVector2(const char * const name, const float x, const float y, const bool useShader = false);
    void SetVector2(const char * const name, const glm::vec2 &value, const bool useShader = false);
    void SetVector3(const char * const name, const float x, const float y, const float z, const bool useShader = false);
    void SetVector3(const char * const name, const glm::vec3 &value, const bool useShader = false);
    void SetVector4(const char * const name, const float x, const float y, const float z, const float w, const bool useShader = false);
    void SetVector4(const char * const name, const glm::vec4 &value, const bool useShader = false);
    void SetMatrix4(const char * const name, const glm::mat4 &matrix, const bool useShader = false);
private:
    // checks if compilation or linking failed and if so, print the error logs
    enum class CompileErrorType {
        VERTEX = 'v',
        FRAGMENT = 'f',
        GEOMETRY = 'g',
        PROGRAM = 'p'
    };
    void checkCompileErrors(unsigned const int object, const CompileErrorType type) const;
};

template<> struct std::hash<Shader>
{
    std::size_t operator()(const Shader& s) const noexcept
    {
        return 67134851221 * s.ID % 46192549927 + 65697490589;
    }
};

#endif