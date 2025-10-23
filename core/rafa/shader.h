#ifndef SHADER_H
#define SHADER_H
#pragma once

#include "header.h"

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    unsigned int use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVector(const std::string& name, float valuex, float valuey, float valuez, float valuew) const;
    void setVector(const std::string &name, float valuex, float valuey, float valuez) const;
    void setVector(const std::string& name, float valuex, float valuey) const;
    void Shader::setMatrix4fv(const std::string& name, const float* value);

};

#endif