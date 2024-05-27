#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include "engine/collider.hpp"
#include "glm/glm.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/model.hpp"
#include <string>
class GameObject {
  public:
    std::string id;
    Model *model;
    Collider *collider;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scaling = glm::vec3(1.0f);
    bool toDestroy = false;

    GameObject(
      std::string const &path,
      glm::vec3 position = glm::vec3(0.0f),
      glm::vec3 rotation = glm::vec3(0.0f),
      glm::vec3 scaling = glm::vec3(1.0f)
    );

    void destroy();
    void addCollider(glm::vec3 lowOffset, glm::vec3 highOffset);
    virtual void renderFrameCallback(float deltaTime);
};

#endif // !DEBUG
