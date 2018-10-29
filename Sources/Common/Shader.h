
#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

class Shader
{
public:
    Shader(std::string path);
    void use();
    void Load(std::string ShaderName, std::string Filename, GLuint& ShaderID);
    GLint getUniform(std::string uniform_name);

    unsigned int id;
    std::string shader_name;

private:
    GLint Result = GL_FALSE;
    int InfoLogLength;
};

class ShaderCreateException : public std::exception
{
public:
    std::string error;
    ShaderCreateException()
    {
        this->error = "";
    }
    ShaderCreateException(std::string shader_name, std::string where)
    {
        this->error = "Exception: shader with name " + shader_name + " failed to load.\n" + where + "\n";
    }
    const char * what() const throw()
    {
        return this->error.c_str();
    }
};

class ShaderUseException : public ShaderCreateException
{
public:
    ShaderUseException(std::string shader_name) : ShaderCreateException()
    {
        this->error = "Exception: cannot use shader with name " + shader_name + ", invalid shader id";
    }
};

#endif


