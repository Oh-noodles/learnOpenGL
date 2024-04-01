#include "engine/scene.hpp"
#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"
#include <cstdlib>
#include <string>
#include <utility>
#include <map>

Scene::Scene(): camera(new Camera()) {
  std::srand(std::time(nullptr));
  id = std::to_string(std::rand());
}

Scene::~Scene() {}

int Scene::addGameObject(GameObject &gameObject) {
  std::srand(std::time(nullptr) * std::rand());
  std::string id = std::to_string(std::rand());
  gameObjects.insert(std::pair<std::string, GameObject&>(id, gameObject));
  return 0;
}

int Scene::addGameObject(
    std::string const &path,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scaling
  ) {
  std::srand(std::time(nullptr) * std::rand());
  std::string id = std::to_string(std::rand());
  GameObject *gameObject = new GameObject(path, position, rotation, scaling);
  gameObjects.insert(std::pair<std::string, GameObject&>(id, *gameObject));
  return 0;
}

int Scene::addLight(Light *light) {
  std::srand(std::time(nullptr) * std::rand());
  std::string id = std::to_string(std::rand());
  std::cout << "addLight: " << id << std::endl;
  lights.insert(std::pair<std::string, Light*>(id, light));
  return 0;
}
