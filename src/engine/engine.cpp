#include <engine/engine.hpp>
/* #include "GLFW/glfw3.h" */
/* #include <OpenGL/OpenGL.h> */
#include <glm/glm.hpp>
/* #include "GLFW/glfw3.h" */
#include "GLFW/glfw3.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "learn/camera.hpp"
#include "learn/shader_s.hpp"
#include "learn/stb_image.h"
/* #include "learn/camera.hpp" */
#include <iostream>

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera *camera);

GLFWwindow* Engine::createWindow(unsigned int width, unsigned int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw: create window
  GLFWwindow *window = glfwCreateWindow(width, height, "quack", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to init GLAD" << std::endl;
    return NULL;
  }

  // set callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);
  return window;
}

Engine::Engine(GLFWwindow *window, unsigned int width, unsigned int height) {
  this->window = window;
  this->width = width;
  this->height = height;
  this->objectShader = new Shader("src/engine/shader.vs", "src/engine/shader.fs");
  this->lightSourceShader = new Shader("src/engine/shader.vs", "src/engine/light_source.fs");
  this->camera = new Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  return;
}

Engine::~Engine() {
  return;
}

void Engine::run() {
  while (!glfwWindowShouldClose(window)) {
    processInput(window, camera);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderObjects();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return;
}

int Engine::addModel(string const &path, float x, float y, float z) {
  models[0] = new Model(path, glm::vec3(x, y, z));
  return 0;
}

void Engine::renderObjects() {
  Shader *shader = objectShader;
  shader->use();
  
  // set view/projection uniform
  glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)width/(float)height, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // set lights
  // directional light
  shader->setVec3("viewPos", camera->position);
  shader->setVec3("dirLight.direction", glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)));
  shader->setVec3("dirLight.ambient", glm::vec3(0.2f));
  shader->setVec3("dirLight.diffuse", glm::vec3(0.5f));
  shader->setVec3("dirLight.specular", glm::vec3(1.0f));
  // set point lights
  shader->setVec3("pointLights[0].position", pointLightPositions[0]);
  shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
  shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
  shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
  shader->setFloat("pointLights[0].constant", 1.0f);
  shader->setFloat("pointLights[0].linear", 0.09f);
  shader->setFloat("pointLights[0].quadratic", 0.032f);

  shader->setVec3("pointLights[1].position", pointLightPositions[1]);
  shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
  shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
  shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
  shader->setFloat("pointLights[1].constant", 1.0f);
  shader->setFloat("pointLights[1].linear", 0.09f);
  shader->setFloat("pointLights[1].quadratic", 0.032f);
  
  shader->setVec3("pointLights[2].position", pointLightPositions[2]);
  shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
  shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
  shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
  shader->setFloat("pointLights[2].constant", 1.0f);
  shader->setFloat("pointLights[2].linear", 0.09f);
  shader->setFloat("pointLights[2].quadratic", 0.032f);

  shader->setVec3("pointLights[3].position", pointLightPositions[3]);
  shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
  shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
  shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
  shader->setFloat("pointLights[3].constant", 1.0f);
  shader->setFloat("pointLights[3].linear", 0.09f);
  shader->setFloat("pointLights[3].quadratic", 0.032f);
  // spot light
  shader->setVec3("spotLight.position", camera->position);
  shader->setVec3("spotLight.direction", camera->front);
  shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  shader->setFloat("spotLight.constant", 1.0f);
  shader->setFloat("spotLight.linear", 0.09f);
  shader->setFloat("spotLight.quadratic", 0.032f);
  shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  // translate and render models
  for (int i = 0; i < 1; i++) {
    if (models[i] == NULL) continue;

    Model loadedModel = *models[i];
    glm::mat4 model = glm::mat4(1.0f);
    model= glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setMat4("model", model);
    loadedModel.draw(*shader);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  std::cout << "framebuffer_size_callback " << width << " " << height << std::endl;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera *camera) {
  float deltaTime = 0.1f;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->ProcessKeyboard(RIGHT, deltaTime);
}
