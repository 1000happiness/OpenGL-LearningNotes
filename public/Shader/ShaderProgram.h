#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <glad/glad.h>

class ShaderProgram
{
private:
    unsigned int ID;
    std::map<GLenum, int> shaderMap;

    bool addStringShader(GLenum shaderType, const char * source);
public:
    enum SourceType {
        FILE,
        STRING,
    };

    int getID();
    bool link();
    void use();

    bool addShader(GLenum shaderType, const char * source, SourceType sourceType);
    int getShader(GLenum);

    bool setUniformValue(const char * name, bool *value, int number);
    bool setUniformValue(const char * name, int *value, int number);
    bool setUniformValue(const char * name, float *value, int number);

    ShaderProgram();
    ~ShaderProgram();
};

#endif