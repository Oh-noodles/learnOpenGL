#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <glad/glad.h>
#include <map>
#include <vector>
#include "GLFW/glfw3.h"
#include "engine/scene.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/camera.hpp"
#include "learn/model.hpp"
#include "learn/shader_s.hpp"

class Engine {
  private:
    static Engine *instance;
    Shader *objectShader;
    Shader *lightSourceShader;
    unsigned int width;
    unsigned int height;
    GLFWwindow *window;
    std::vector<Model*> models;
    std::map<std::string, Scene&> scenes;
    Scene *activeScene = NULL;
  public:
    static GLFWwindow* createWindow(unsigned int width, unsigned int height);
    static Engine* getInstance();
    Engine(GLFWwindow *window, unsigned int width, unsigned int height);
    ~Engine();

    void (*renderFrameCallback)(float) = NULL;

    void run();
    int addScene();
    Scene& getActiveScene();
    int addGameObject(
        std::string const &path,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scaling = glm::vec3(1.0f)
      );
    void renderObjects();
};

#endif // !DEBUG
