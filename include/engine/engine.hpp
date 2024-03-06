#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "learn/camera.hpp"
#include "learn/model.hpp"
#include "learn/shader_s.hpp"

#define MX_NR_MODEL 10

class Engine {
  private:
    Shader *objectShader;
    Shader *lightSourceShader;
    Camera *camera;
    unsigned int width;
    unsigned int height;
    GLFWwindow *window;
    Model* models[MX_NR_MODEL] = {};
  public:
    static GLFWwindow* createWindow(unsigned int width, unsigned int height);
    Engine(GLFWwindow *window, unsigned int width, unsigned int height);
    ~Engine();

    void run();
    int addModel(string const &path, float x, float y, float z);
    void renderObjects();
};

#endif // !DEBUG
