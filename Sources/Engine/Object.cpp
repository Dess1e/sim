#include "Object.h"
#include <Sources/Engine/Engine.h>


Object::Object(glm::vec3 initial_coords, glm::vec3 init_rotations_deg, glm::vec3 initial_scale, std::string mesh_path)
{
    auto model_mat = glm::mat4(1);
    //translate
    model_mat = glm::translate(model_mat, initial_coords);
    //rotate (for every angle)
    for (int i = 0; i < 3; ++i)
    {
        auto v = glm::vec3(0);
        v[i] = 1;
        model_mat = glm::rotate(model_mat, glm::radians(init_rotations_deg[i]), v);
    }
    //scale
    model_mat = glm::scale(model_mat, initial_scale);
    this->model_matrix = model_mat;

    this->object_mesh = new Model(mesh_path);
}

Object::Object(std::string mesh_path)
{
    //this->internal_name =
    this->model_matrix = glm::mat4(1);
    this->object_mesh = new Model(mesh_path);
}

void Object::render()
{
    auto engine = Engine::getEngine();
    GLuint MatrixID = engine->gl_variables->current_shader->getUniform("model_projection_mat");
    auto mvp = calculateMVP(16/9, 0.1, 100);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    this->object_mesh->draw(engine->gl_variables->current_shader);
}

glm::mat4 Object::calculateMVP(float ratio, float nearz, float farz)
{
    auto engine = Engine::getEngine();
    glm::mat4 proj = glm::perspective(glm::radians(engine->PlayerObject->player_fov), ratio, nearz, farz);
    glm::mat4 view = glm::lookAt(engine->PlayerObject->player_pos,
                                 engine->PlayerObject->direction + engine->PlayerObject->player_pos,
                                 engine->PlayerObject->up);
    glm::mat4 model = this->getModelMatrix(); //to be removed
    glm::mat4 mvp = proj * view * model;
    return mvp;
}

void Object::translate(glm::vec3 translation_vec)
{
    this->model_matrix = glm::translate(this->getModelMatrix(), translation_vec);
}

void Object::rotate(glm::vec3 rotation_vec)
{
    for (int i = 0; i < 3; ++i)
    {
        auto v = glm::vec3(0);
        v[i] = 1;
        this->model_matrix = glm::rotate(this->getModelMatrix(), glm::radians(rotation_vec[i]), v);
    }
}

void Object::scale(glm::vec3 scale_vec)
{
    this->model_matrix = glm::scale(this->getModelMatrix(), scale_vec);
}

glm::mat4 Object::getModelMatrix() const
{
   return this->model_matrix;
}
