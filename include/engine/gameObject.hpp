#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include "glm/glm.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/model.hpp"
#include <string>
class GameObject {
  public:
    Model *model;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scaling = glm::vec3(1.0f);
    GameObject(
      std::string const &path,
      glm::vec3 position = glm::vec3(0.0f),
      glm::vec3 rotation = glm::vec3(0.0f),
      glm::vec3 scaling = glm::vec3(1.0f)
    );
};

#endif // !DEBUG
