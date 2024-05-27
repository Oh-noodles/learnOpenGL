#include "engine/gameObject.hpp"
#include "engine/collider.hpp"
#include "engine/utils.hpp"
#include "glm/detail/type_vec.hpp"
#include <string>

std::string genId() {
  std::srand(std::time(nullptr) * std::rand());
  std::string id = std::to_string(std::rand());
  return id;
}

GameObject::GameObject(
    std::string const &path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scaling
  ) {
  this->id = Utils::uuid();
  this->position = position;
  this->rotation = rotation;
  this->scaling = scaling;
  this->model = new Model(path);
}

void GameObject::addCollider(glm::vec3 lowOffset, glm::vec3 highOffset) {
  collider = new Collider(position, lowOffset, highOffset);
}

// TODO: destroy the gameObject really
void GameObject::destroy() {
  std::cout << "this gameObject need to be destroyed" << std::endl;
  toDestroy = true;
}

void GameObject::renderFrameCallback(float deltaTime) {}
