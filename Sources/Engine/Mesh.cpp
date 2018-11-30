#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices,
           const std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->vao_id);
    glGenBuffers(1, &this->vbo_id);
    glGenBuffers(1, &this->ebo_id);

    glBindVertexArray(this->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::draw(Shader *pShader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (unsigned int i = 0; i < this->textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number, name = this->textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        auto uniform = pShader->getUniform(name + number);
        glUniform1i(uniform, static_cast<GLint>(i));
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glBindVertexArray(this->vao_id);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}












