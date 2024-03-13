#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <glad/glad.h>
#include <map>
#include <vector>
#include "GLFW/glfw3.h"
#include "engine/scene.hpp"
#include "learn/camera.hpp"
#include "learn/model.hpp"
#include "learn/shader_s.hpp"

class Engine {
  protected:
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

    void run();
    int addScene();
    Scene& getActiveScene();
    int addModel(string const &path, float x, float y, float z);
    void renderObjects();
};

#endif // !DEBUG
