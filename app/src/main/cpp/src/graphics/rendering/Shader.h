#ifndef ANDROIDGLINVESTIGATIONS_SHADER_H
#define ANDROIDGLINVESTIGATIONS_SHADER_H


#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class Model;

/*!
 * A class representing a simple shader program. It consists of vertex and fragment components. The
 * input attributes are a position (as a Vector3) and a uv (as a Vector2). It also takes a uniform
 * to be used as the entire model/view/projection matrix. The shader expects a single texture for
 * fragment shading, and does no other lighting calculations (thus no uniforms for lights or normal
 * attributes).
 */

class Shader
{
public:
    //thie program ID
    unsigned int ID;

    Shader();
    //constructor read and builds the shdaer
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    //use/activate the shader
    Shader& use();
    //utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatArray(const std::string& name, float* firstvalue,int size) const;
    void setIntArray(const std::string& name, int* array, int size) const;
    void setMat4(const std::string& name, glm::mat4 matrix, bool useShader = false);
    void setivec2(const std::string& name, glm::ivec2 value);
    void setvec2(const std::string& name, glm::vec2 value);
    void setvec4(const std::string& name, glm::vec4 value);
    void setvec3f(const std::string& name, glm::vec3 value);
};

#endif //ANDROIDGLINVESTIGATIONS_SHADER_H