#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <Sources/Engine/Model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Sources/Engine/AssetsManager.h>

#define MVP_MATRIX_UNIFORM "model_projection_mat"

class Engine;

class Object
{

public:
    Object(glm::vec3 initial_coords, glm::vec3 initial_rot, glm::vec3 initial_scale, std::string asset_alias, uint64_t id);
    Object(std::string asset_alias);
    void render();
    void translate(glm::vec3 translation_vec);
    void rotate(glm::vec3 rotation_vec);
    void scale(glm::vec3 scale_vec);
    void recalculateModelMatrix();
    glm::mat4 getModelMatrix() const;
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);
    uint64_t getObjectID();
    std::string getObjectAssetAlias();
private:
    Model * object_mesh;
    glm::mat4 model_matrix;
    glm::vec3 coords3;
    glm::vec3 rotation3;
    glm::vec3 scale3;
    std::string asset_alias;
    uint64_t object_id;
};

#endif // OBJECT_HPP
