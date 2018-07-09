#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>

class Vertex
{
public:
    Vertex();
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

#endif // VERTEX_H
