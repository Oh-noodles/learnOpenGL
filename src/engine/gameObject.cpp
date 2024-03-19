#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"
#include <string>

GameObject::GameObject(
    std::string const &path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scaling
  ) {
  this->position = position;
  this->rotation = rotation;
  this->scaling = scaling;
  this->model = new Model(path);
}

 void GameObject::renderFrameCallback(float deltaTime) {}
