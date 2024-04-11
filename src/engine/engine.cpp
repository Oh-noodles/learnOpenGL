#include <engine/engine.hpp>
/* #include "GLFW/glfw3.h" */
/* #include <OpenGL/OpenGL.h> */
#include <glm/glm.hpp>
/* #include "GLFW/glfw3.h" */
#include "GLFW/glfw3.h"
#include "engine/gameObject.hpp"
#include "engine/scene.hpp"
#include "game/tank.hpp"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "learn/camera.hpp"
#include "learn/shader_s.hpp"
#include "learn/stb_image.h"
/* #include "learn/camera.hpp" */
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

unsigned int pointLightIdx = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera &camera, float deltaTime);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

float deltaTime = 0.1f;
float lastFrameTime = static_cast<float>(glfwGetTime());

Engine* Engine::instance = NULL;

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  // tell GLFW to capture our mosue
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);
  return window;
}


Engine* Engine::getInstance() {
  return instance;
}

Engine::Engine(GLFWwindow *window, unsigned int width, unsigned int height) {
  if (instance != NULL) {
    std::cout << "ERROR::Engine should be singlton";
    /* throw "Engine shoudle be singlton"; */
    return;
  };

  this->window = window;
  this->width = width;
  this->height = height;
  objectShader = new Shader("src/engine/shader.vs", "src/engine/shader.fs");
  lightSourceShader = new Shader("src/engine/shader.vs", "src/engine/light_source.fs");
  instance = this;
  return;
}

Engine::~Engine() {
  return;
}

int Engine::addScene() {
  Scene *scene = new Scene();
  scenes.insert({scene->id, scene});
  activeScene = scene;
  return 0;
}

Scene* Engine::getActiveScene() {
  return activeScene;
}

void Engine::run() {

  while (!glfwWindowShouldClose(window)) {
    if (activeScene == NULL) continue;

    /* lastFrameTime = static_cast<float>(glfwGetTime()); */
    float currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;


    processInput(window, *(activeScene->camera), deltaTime);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderObjects();

    if (renderFrameCallback != NULL) renderFrameCallback(deltaTime);
    if (activeScene->renderFrameCallback != NULL) activeScene->renderFrameCallback(deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

  glfwTerminate();
  return;
}

/* int Engine::addGameObject( */
/*     string const &path, */
/*     glm::vec3 position, */
/*     glm::vec3 rotation, */
/*     glm::vec3 scaling */
/*   ) { */
/*   if (activeScene == NULL) { */
/*     std::cout << "no active scene" << std::endl; */
/*     return -1; */
/*   } */
/*   activeScene->addGameObject(path, position, rotation, scaling); */
/*   std::cout << "gameObject added" << std::endl; */
/*   return 0; */
/* } */

void setupLight(Shader *shader, Camera *camera, Light *light) {
  glm::vec3 vec;
  switch (light->type) {
    case DIR:
      /* direction = ((DirectionalLight*)light)->ambient; */
      /* std::cout << "direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl; */
      shader->setVec3("dirLight.direction", ((DirectionalLight*)light)->direction);
      shader->setVec3("dirLight.ambient", ((DirectionalLight*)light)->ambient);
      shader->setVec3("dirLight.diffuse", ((DirectionalLight*)light)->diffuse);
      shader->setVec3("dirLight.specular", ((DirectionalLight*)light)->specular);
      break;
    case POINT:
      /* direction = ((DirectionalLight*)light)->ambient; */
      /* std::cout << "point: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl; */
      shader->setVec3(string("pointLights[") + to_string(pointLightIdx) + string("].position"), ((PointLight*)light)->position);
      shader->setVec3(string("pointLights[") + to_string(pointLightIdx) + string("].ambient"), ((PointLight*)light)->ambient);
      shader->setVec3(string("pointLights[") + to_string(pointLightIdx) + string("].diffuse"), ((PointLight*)light)->diffuse);
      shader->setVec3(string("pointLights[") + to_string(pointLightIdx) + string("].specular"), ((PointLight*)light)->specular);
      shader->setFloat(string("pointLights[") + to_string(pointLightIdx) + string("].constant"), ((PointLight*)light)->constant);
      shader->setFloat(string("pointLights[") + to_string(pointLightIdx) + string("].linear"), ((PointLight*)light)->linear);
      shader->setFloat(string("pointLights[") + to_string(pointLightIdx) + string("].quadratic"), ((PointLight*)light)->quadratic);
      pointLightIdx++;
      break;
    case SPOT:
      shader->setVec3("spotLight.position", camera->position);
      shader->setVec3("spotLight.direction", camera->front);
      vec = camera->position;
      shader->setVec3("spotLight.ambient", ((SpotLight*)light)->ambient);
      shader->setVec3("spotLight.diffuse", ((SpotLight*)light)->diffuse);
      shader->setVec3("spotLight.specular", ((SpotLight*)light)->specular);
      shader->setFloat("spotLight.constant", ((SpotLight*)light)->constant);
      shader->setFloat("spotLight.linear", ((SpotLight*)light)->linear);
      shader->setFloat("spotLight.quadratic", ((SpotLight*)light)->quadratic);
      shader->setFloat("spotLight.cutOff", ((SpotLight*)light)->cutOff);
      shader->setFloat("spotLight.outerCutOff", ((SpotLight*)light)->outerCutOff);
      break;
    default:
      break;
  }
}

void Engine::renderObjects() {
  if (activeScene == NULL) return;


  Shader *shader = objectShader;
  shader->use();
  
  // set view/projection uniform
  Camera *camera = getActiveScene()->camera;
  glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)width/(float)height, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // set lights
  // directional light
  shader->setVec3("viewPos", camera->position);
  
  pointLightIdx = 0;
  for (auto &lt: activeScene->lights) {
    Light *light = lt.second;
    setupLight(shader, camera, light);
  }

  // translate and render models
  for (auto it = activeScene->gameObjects.cbegin(); it != activeScene->gameObjects.cend(); ) {
    std::string id = it->first;
    GameObject *gameObject = it->second;
    it++;

    if (!gameObject) continue;
    if (gameObject->toDestroy) {
      activeScene->removeGameObject(id);
      continue;
    } 

    Model *loadedModel = gameObject->model;
    gameObject->renderFrameCallback(deltaTime);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, gameObject->position);
    model = glm::scale(model, gameObject->scaling);
    model = glm::rotate(model, gameObject->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, gameObject->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, gameObject->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    shader->setMat4("model", model);
    loadedModel->draw(*shader);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  std::cout << "framebuffer_size_callback " << width << " " << height << std::endl;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera &camera, float deltaTime) {
  /* static float deltaTime = 0.1f; */
  /* static float lastFrameTime = static_cast<float>(glfwGetTime()); */
  /* float currentFrameTime = static_cast<float>(glfwGetTime()); */
  /* deltaTime = currentFrameTime - lastFrameTime; */
  /* lastFrameTime = currentFrameTime; */

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  /* if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(FORWARD, deltaTime); */
  /* if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(BACKWARD, deltaTime); */
  /* if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(LEFT, deltaTime); */
  /* if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(RIGHT, deltaTime); */
  /* if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(UP, deltaTime); */
  /* if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) */
  /*   camera.ProcessKeyboard(DOWN, deltaTime); */
}

void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn) {

  Engine *instance = Engine::getInstance();
  if (!instance) return;

  Scene *scene = instance->getActiveScene();
  if (!scene) return;

  // mouseCallback of current active scene
  if (scene->mouseCallback) {
    scene->mouseCallback(window, xPosIn, yPosIn);
  }

  /* static bool firstMouse = false; */
  /* static float lastX = 0; */
  /* static float lastY = 0; */
  /**/
  /* float xPos = static_cast<float>(xPosIn); */
  /* float yPos = static_cast<float>(yPosIn); */
  /**/
  /* if (firstMouse) { */
  /*   lastX = xPos; */
  /*   lastY = yPos; */
  /*   firstMouse = false; */
  /* } */
  /**/
  /* float xOffset = xPos - lastX; */
  /* float yOffset = lastY - yPos; */
  /**/
  /* lastX = xPos; */
  /* lastY = yPos; */
  /**/
  /* Engine* engine = Engine::getInstance(); */
  /* if (engine != NULL) { */
  /*   engine->getActiveScene()->camera.ProcessMouseMovement(xOffset, yOffset); */
  /* } */
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
  Engine* engine = Engine::getInstance();
  if (engine != NULL) {
    engine->getActiveScene()->camera->ProcessMouseScroll(yOffset);
  }
}
