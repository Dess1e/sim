#ifndef LANDSCAPE_H
#define LANDSCAPE_H
#include "src/engine/Vertex.h"
#include <src/common/Shader.h>
#include <src/engine/Mesh.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <vector>

class Landscape
{
public:
    Landscape();

    Mesh CreateBasicShape();
private:
    std::vector<Vertex> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
};

#endif // LANDSCAPE_H