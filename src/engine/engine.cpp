#include <engine/engine.hpp>
/* #include "GLFW/glfw3.h" */
/* #include <OpenGL/OpenGL.h> */
#include <glm/glm.hpp>
/* #include "GLFW/glfw3.h" */
#include "GLFW/glfw3.h"
#include "engine/scene.hpp"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera *camera);
/* void mouse_callback(GLFWwindow *window, double xPos, double yPos); */
/* void scroll_callback(GLFWwindow *window, double xOffset, double yOffset); */

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
  this->camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  return;
}

Engine::~Engine() {
  return;
}

int Engine::addScene() {
  Scene *scene = new Scene();
  scenes.insert(std::pair<std::string, Scene&>(scene->id, *scene));
  activeScene = scene;
  return 0;
}

Scene& Engine::getActiveScene() {
  return *activeScene;
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
  if (activeScene == NULL) {
    std::cout << "no active scene" << std::endl;
    return -1;
  }
  activeScene->addGameObject(path, x, y, z);
  std::cout << "gameObject added" << std::endl;
  return 0;
  /* Model *model = new Model(path, glm::vec3(x, y, z)); */
  /* models.push_back(model); */
  /* return 0; */
}

void setupLight(Shader *shader, Camera *camera, Light *light) {
  glm::vec3 vec;
  std::cout << "light type: " << light->type << std::endl;
  unsigned int pointLightIdx = 0;
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
      /* shader->setVec3("pointLights[0].ambient", ((PointLight*)light)->ambient); */
      /* shader->setVec3("pointLights[0].diffuse", ((PointLight*)light)->diffuse); */
      /* shader->setVec3("pointLights[0].specular", ((PointLight*)light)->specular); */
      /* shader->setFloat("pointLights[0].constant", ((PointLight*)light)->constant); */
      /* shader->setFloat("pointLights[0].linear", ((PointLight*)light)->linear); */
      /* shader->setFloat("pointLights[0].quadratic", ((PointLight*)light)->quadratic); */
      break;
    case SPOT:
      shader->setVec3("spotLight.position", camera->position);
      shader->setVec3("spotLight.direction", camera->front);
      vec = camera->position;
      std::cout << "camera.position: " << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
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
  glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)width/(float)height, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // set lights
  // directional light
  shader->setVec3("viewPos", camera->position);
  
  std::cout << "light count: " << activeScene->lights.size() << std::endl;
  for (auto &lt: activeScene->lights) {
    Light *light = lt.second;
    setupLight(shader, camera, light);
  }

  /* // directional light */
  /* shader->setVec3("dirLight.direction", glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f))); */
  /* shader->setVec3("dirLight.ambient", glm::vec3(0.2f)); */
  /* shader->setVec3("dirLight.diffuse", glm::vec3(0.5f)); */
  /* shader->setVec3("dirLight.specular", glm::vec3(1.0f)); */
  /**/
  /* // set point lights */
  /* shader->setVec3("pointLights[0].position", pointLightPositions[0]); */
  /* shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f); */
  /* shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f); */
  /* shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f); */
  /* shader->setFloat("pointLights[0].constant", 1.0f); */
  /* shader->setFloat("pointLights[0].linear", 0.09f); */
  /* shader->setFloat("pointLights[0].quadratic", 0.032f); */
  /**/
  /* shader->setVec3("pointLights[1].position", pointLightPositions[1]); */
  /* shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f); */
  /* shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f); */
  /* shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f); */
  /* shader->setFloat("pointLights[1].constant", 1.0f); */
  /* shader->setFloat("pointLights[1].linear", 0.09f); */
  /* shader->setFloat("pointLights[1].quadratic", 0.032f); */
  /**/
  /* shader->setVec3("pointLights[2].position", pointLightPositions[2]); */
  /* shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f); */
  /* shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f); */
  /* shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f); */
  /* shader->setFloat("pointLights[2].constant", 1.0f); */
  /* shader->setFloat("pointLights[2].linear", 0.09f); */
  /* shader->setFloat("pointLights[2].quadratic", 0.032f); */
  /**/
  /* shader->setVec3("pointLights[3].position", pointLightPositions[3]); */
  /* shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f); */
  /* shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f); */
  /* shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f); */
  /* shader->setFloat("pointLights[3].constant", 1.0f); */
  /* shader->setFloat("pointLights[3].linear", 0.09f); */
  /* shader->setFloat("pointLights[3].quadratic", 0.032f); */
  /* // spot light */
  /* shader->setVec3("spotLight.position", camera->position); */
  /* shader->setVec3("spotLight.direction", camera->front); */
  /* shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f); */
  /* shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f); */
  /* shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); */
  /* shader->setFloat("spotLight.constant", 1.0f); */
  /* shader->setFloat("spotLight.linear", 0.09f); */
  /* shader->setFloat("spotLight.quadratic", 0.032f); */
  /* shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f))); */
  /* shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); */

  // translate and render models
  for (auto &it: activeScene->gameObjects) {
    Model &loadedModel = it.second;
    glm::mat4 model = glm::mat4(1.0f);
    model= glm::translate(model, loadedModel.position);
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

/* void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn) { */
/*   static bool firstMouse = false; */
/*   static float lastX = 0; */
/*   static float lastY = 0; */
/**/
/*   float xPos = static_cast<float>(xPosIn); */
/*   float yPos = static_cast<float>(yPosIn); */
/**/
/*   if (firstMouse) { */
/*     lastX = xPos; */
/*     lastY = yPos; */
/*     firstMouse = false; */
/*   } */
/**/
/*   float xOffset = xPos - lastX; */
/*   float yOffset = lastY - yPos; */
/**/
/*   lastX = xPos; */
/*   lastY = yPos; */
/**/
/*   camera.ProcessMouseMovement(xOffset, yOffset); */
/* } */
