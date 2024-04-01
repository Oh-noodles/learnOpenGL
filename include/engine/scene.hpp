#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/camera.hpp"
#include "learn/model.hpp"
#include <map>
#include <string>

enum LightType {
  DIR,
  POINT,
  SPOT
};

struct Light {
  LightType type;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  Light(glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 diffuse = glm::vec3(0.0f), glm::vec3 specular = glm::vec3(0.0f)) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
  }
};

struct DirectionalLight: Light {
  /* LightType type = DIR; */
  glm::vec3 direction;

  DirectionalLight() {
    type = DIR;
  }

  DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction): Light(ambient, diffuse, specular) {
    type = DIR;
    this->direction = direction;
  }
};

struct PointLight: Light {
  glm::vec3 position;

  float constant;
  float linear;
  float quadratic;

  PointLight() {
    type = POINT;
  }

  PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic): Light(ambient, diffuse, specular) {
    type = POINT;
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
  }
};

struct SpotLight: Light {
  glm::vec3 position;
  glm::vec3 direction;

  float cutOff;
  float outerCutOff;
  float constant;
  float linear;
  float quadratic;

  SpotLight() {
    type = SPOT;
  }
};

class Scene {
  private:
  public:
    Camera *camera;
    std::map<std::string, Light*> lights;
    std::map<std::string, GameObject&> gameObjects;
    std::string id;
    Scene();
    ~Scene();
    int addGameObject(GameObject &gameObject);
    int addGameObject(
        std::string const &path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scaling = glm::vec3(1.0f)
      );
    int addLight(Light *light);

    void (*mouseCallback)(GLFWwindow *window, double xPosIn, double yPosIn) = NULL;
    void (*renderFrameCallback)(float deltaTime) = NULL;
};

#endif // !__SCENE_HPP__
