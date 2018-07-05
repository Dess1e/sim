
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
    void Use();
    unsigned int id;
    std::string shader_name;
};

class ShaderCreateException : public std::exception
{
public:
    std::string error;
    ShaderCreateException(std::string shader_name, std::string where)
    {
        this->error = "Exception: shader with name " + shader_name + " failed to load.\n" + where + "\n";
    }
    const char * what() const throw()
    {
        return this->error.c_str();
    }
};


#endif


