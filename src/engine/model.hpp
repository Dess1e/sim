#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Model
{
    Model(GLfloat[]);
public:
    std::string modelName;
    const GLfloat * vertices;
    const GLfloat * colors;
    glm::mat4 mvp;
};
#endif
