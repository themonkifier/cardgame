#include "../include/shader.hh"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    /* open files */
    FILE* vertexFile;
    FILE* fragmentFile;
    
    if ((vertexFile = fopen(vertexPath, "r")) == NULL)
    {
        printf("Failed to open vertex shader\n");
        return;
    }
    else if ((fragmentFile = fopen(fragmentPath, "r")) == NULL)
    {
        fclose(vertexFile);
        printf("Failed to open fragmentation shader\n");
        return;
    }
    
    /* get length of files */
    fseek(vertexFile, 0, SEEK_END);
    long int vertexLength = ftell(vertexFile);
    rewind(vertexFile);
    
    fseek(fragmentFile, 0, SEEK_END);
    long int fragmentLength = ftell(fragmentFile);
    rewind(fragmentFile);
    
    /* read files into buffers */
    char* vertexCode = (char*) malloc((vertexLength + 1) * sizeof(char));
    char* fragmentCode = (char*) malloc((fragmentLength + 1) * sizeof(char));
        
    fread((void*) vertexCode, sizeof(char), vertexLength, vertexFile);
    vertexCode[vertexLength] = '\0';
    fclose(vertexFile);
    
    fread((void*) fragmentCode, sizeof(char), fragmentLength, fragmentFile);
    fragmentCode[fragmentLength] = '\0';
    fclose(fragmentFile);
    
    /* setup shaders */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char* const*) &vertexCode, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Failed to compile vertex shader\n%s\n", infoLog);
    }
    free(vertexCode);
    
    /* fragment shader */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char* const*) &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Failed to compile fragmentation shader\n%s\n", infoLog);
    }
    free(fragmentCode);
    
    /* link shaders */
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    // check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        printf("Failed to link shader program\n%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}


void Shader::set_int(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::set_float(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::set_bool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name), (int) value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const char* name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]); 
}

void Shader::setVec2(const char* name, float x, float y) const
{ 
    glUniform2f(glGetUniformLocation(ID, name), x, y); 
}
// ------------------------------------------------------------------------
void Shader::setVec3(const char* name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]); 
}

void Shader::setVec3(const char* name, float x, float y, float z) const
{ 
    glUniform3f(glGetUniformLocation(ID, name), x, y, z); 
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]); 
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{ 
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w); 
}
// ------------------------------------------------------------------------
void Shader::setMat3(const char* name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const char* name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}