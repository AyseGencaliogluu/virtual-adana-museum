#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// A utility class to manage vertex and fragment shaders
class Shader {
public:
    // ID of the compiled shader program
    unsigned int ID;

    // Constructor: builds the shader program from vertex and fragment shader file paths
    Shader(const char* vertexPath, const char* fragmentPath);

    // Activate the shader program
    void use();

    // Utility functions to set shader uniform variables
    void setBool(const std::string& name, bool value) const;     // Set a boolean uniform
    void setInt(const std::string& name, int value) const;       // Set an integer uniform
    void setFloat(const std::string& name, float value) const;   // Set a float uniform
    void setVec3(const std::string& name, const glm::vec3& value) const; // Set a vec3 uniform (e.g., position, color)
    void setMat4(const std::string& name, const glm::mat4& mat) const;   // Set a 4x4 matrix uniform (e.g., transformations)
};

#endif
