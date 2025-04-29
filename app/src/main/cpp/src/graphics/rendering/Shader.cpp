#include "Shader.h"
#include <sstream>  // for reading shaders
#include<glm/gtc/type_ptr.hpp>
#include "../../AndroidOut.h"
#include "../ResourceManager.h"

Shader::Shader()
{
}

Shader::Shader( const std::string& vertexPath, const std::string& fragmentPath)
{
    char* vertexShaderSource;
    char* fragmentShaderSource;
    AAsset* VertexAsset = AAssetManager_open(ResourceManager::getAssetManeger(), vertexPath.c_str(), AASSET_MODE_BUFFER);
if(!VertexAsset){
    aout<<"VERTEX::ASSET::ERROR"<<std::endl;
}
    AAsset* FragmentAsset = AAssetManager_open(ResourceManager::getAssetManeger(), fragmentPath.c_str(), AASSET_MODE_BUFFER);
    if(!FragmentAsset){
        aout<<"FRAGMENT::ASSET::ERROR"<<std::endl;
    }

    try {

         //
        size_t vertSize = AAsset_getLength(VertexAsset);
         vertexShaderSource = new char[vertSize + 1];
        AAsset_read(VertexAsset, vertexShaderSource, vertSize);
        vertexShaderSource[vertSize] = '\0';
        AAsset_close(VertexAsset);
        //
        size_t fragSize = AAsset_getLength(FragmentAsset);
        fragmentShaderSource = new char [fragSize + 1];
        AAsset_read(FragmentAsset,fragmentShaderSource,fragSize);
        fragmentShaderSource[fragSize]  = '\0';
        AAsset_close(FragmentAsset);


    }
    catch (std::exception e)
    {
        aout << "ERROR::SHADER: Failed to read shader file" << std::endl;

    }

  //  const char* vertexShaderSource = VertexShaderCode.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        aout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    /* */

   // const char* fragmentShaderSource = FragmentShaderCode.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        aout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    /* */

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);


    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        aout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

    }
    glUseProgram(ID);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    delete[] vertexShaderSource;
    delete[] fragmentShaderSource;
}

Shader& Shader::use()
{
    glUseProgram(ID);
    return *this;
}

void Shader::setBool(const std::string& name, bool value) const
{

    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1i(timeValueLocation, value);
}

void Shader::setInt(const std::string& name, int value) const
{

    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1i(timeValueLocation, value);
}

void Shader::setFloat(const std::string& name, float value) const
{

    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1f(timeValueLocation, value);
}

void Shader::setFloatArray(const std::string& name, float* firstvalue,int size) const
{
    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1fv(timeValueLocation,size, firstvalue);

}

void Shader::setIntArray(const std::string& name, int* array, int size) const
{
    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1iv(timeValueLocation, size, array);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix, bool useShader)
{
    if (useShader)
        use();

    int timeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(timeValueLocation,1,GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setivec2(const std::string& name, glm::ivec2 value){

    int tilmeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform2i(tilmeValueLocation, value.x, value.y);
}

void Shader::setvec2(const std::string& name, glm::vec2 value)
{
    int tilmeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform2f(tilmeValueLocation, value.x, value.y);
}

void Shader::setvec4(const std::string& name, glm::vec4 value)
{

    int tilmeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform4f(tilmeValueLocation, value.x, value.y,value.z,value.a);
}

void Shader::setvec3f(const std::string& name, glm::vec3 value)
{
    int tilmeValueLocation = glGetUniformLocation(ID, name.c_str());
    glUniform3f(tilmeValueLocation, value.x, value.y, value.z);
}
