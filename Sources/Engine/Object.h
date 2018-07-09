#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "Mesh.h"


class Object
{
public:
    Object();
    void translate();
    void rotate();
    void scale();
private:
    Mesh * object_mesh;
    std::string internal_name;
    glm::vec4 model_matrix;
};

#endif // OBJECT_HPP
