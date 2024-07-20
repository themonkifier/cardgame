#include "../include/shader.hh"

Shader::Shader(const char * const vertexPath, const char * const fragmentPath, const char * const geometryPath) {
    // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr) {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, CompileErrorType::VERTEX);
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, CompileErrorType::FRAGMENT);
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr) {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, CompileErrorType::GEOMETRY);
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr) glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, CompileErrorType::PROGRAM);
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) glDeleteShader(geometry);
}

Shader &Shader::Use() {
    glUseProgram(ID);
    return *this;
}

void Shader::SetFloat(const char * const name, const float value, const bool useShader) {
    if (useShader) Use();
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetInteger(const char * const name, const int value, const bool useShader) {
    if (useShader) Use();
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetVector2(const char * const name, const float x, const float y, const bool useShader) {
    if (useShader) Use();
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void Shader::SetVector2(const char * const name, const glm::vec2 &value, const bool useShader) {
    if (useShader) Use();
    glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}

void Shader::SetVector3(const char * const name, const float x, const float y, const float z, const bool useShader) {
    if (useShader) Use();
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::SetVector3(const char * const name, const glm::vec3 &value, const bool useShader) {
    if (useShader) Use();
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4(const char * const name, const float x, const float y, const float z, const float w, const bool useShader) {
    if (useShader) Use();
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}

void Shader::SetVector4(const char * const name, const glm::vec4 &value, const bool useShader) {
    if (useShader) Use();
    glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char * const name, const glm::mat4 &matrix, const bool useShader) {
    if (useShader) Use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned const int object, const CompileErrorType type) const {
    int success;
    char infoLog[1024];
    if (type != CompileErrorType::PROGRAM) {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER: Compile-time error: Type: " << static_cast<char>(type) << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::Shader: Link-time error: Type: " << static_cast<char>(type) << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}

std::size_t Shader::operator()(const Shader& s) const noexcept {
    return 67134851221 * s.ID % 46192549927 + 65697490589;
}
