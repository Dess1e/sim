#ifndef MESH_H
#define MESH_H
#include "Texture.h"
#include "Vertex.h"
#include <vector>
#include <GL/glew.h>
#include <cstddef>
#include <Sources/Common/Shader.h>

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<Texture>& textures);

    void draw(Shader * pShader);
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    GLuint vao_id;
    
private:
    GLuint vbo_id, ebo_id;
    void setupMesh();
};

#endif // MESH_H
