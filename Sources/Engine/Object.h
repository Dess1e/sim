#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <Sources/Engine/Model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MVP_MATRIX_UNIFORM "model_projection_mat"

class Engine;

class Object
{

public:
    Object(glm::vec3 initial_coords, glm::vec3 initial_rot, glm::vec3 initial_scale, std::string mesh_path);
    Object(std::string mesh_path);
    void render();
    void translate(glm::vec3 translation_vec);
    void rotate(glm::vec3 rotation_vec);
    void scale(glm::vec3 scale_vec);
    glm::mat4 getModelMatrix() const;
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);
private:
    Model * object_mesh;
    std::string internal_name;
    glm::mat4 model_matrix;
};

#endif // OBJECT_HPP
