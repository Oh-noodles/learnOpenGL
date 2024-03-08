#include "engine/scene.hpp"
#include <cstdlib>
#include <string>
#include <utility>
#include <map>

Scene::Scene(): camera(*(new Camera())) {
  std::srand(std::time(nullptr));
  id = std::to_string(std::rand());
}

Scene::~Scene() {}

int Scene::addGameObject(std::string const &path, float x, float y, float z) {
  std::srand(std::time(nullptr));
  std::string id = std::to_string(std::rand());
  Model *model = new Model(path, glm::vec3(x, y, z));
  gameObjects.insert(std::pair<std::string, Model&>(id, *model));
  return 0;
}

int Scene::addLight(Light light) {
  std::srand(std::time(nullptr));
  std::string id = std::to_string(std::rand());
  lights.insert(std::pair<std::string, Light>(id, light));
  return 0;
}
