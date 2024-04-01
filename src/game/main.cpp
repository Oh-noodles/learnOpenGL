#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "engine/engine.hpp"
#include "engine/scene.hpp"
#include "game/enemy.hpp"
#include "game/player.hpp"
#include "game/tank.hpp"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtx/vector_angle.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow *gWindow;
Player *gPlayer;
TANK::Tank *tank01;
std::vector<Enemy *> enemies;

void renderFrameCallback(float deltaTime) {
  // we do collision detection in frame
  for (int i = 0; i < enemies.size(); i++) {
    bool collided = false;
    for (int j = 0; j < enemies.size(); j++) {
      if (i == j)
        continue;
      // check if target enemy is in front of current enemy
      glm::vec3 pos = enemies[i]->position;
      glm::vec3 front = enemies[i]->front;
      glm::vec3 target = enemies[j]->position;
      float angle =
          glm::orientedAngle(target - pos, front, glm::vec3(0.0f, 1.0f, 0.0f));
      float dist = glm::length(target - pos);
      if (dist < 10 && angle > -0.5 && angle < 0.5) {
        collided = true;
        break;
      }
    }
    enemies[i]->collided = collided;
  }
}

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn);
void processInput(float deltaTime);

int main() {
  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

  GLFWwindow *window = Engine::createWindow(SCR_WIDTH, SCR_HEIGHT);
  gWindow = window;
  Engine engine(window, SCR_WIDTH, SCR_HEIGHT);
  engine.addScene();
  engine.getActiveScene()->addGameObject(
      "resources/objects/backpack/backpack.obj", glm::vec3(-2.0f, 0.0f, 0.0f));
  /* engine.getActiveScene().addGameObject("resources/objects/simple_tank/scene.gltf",
   * glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.01f)); */

  /* tank01 = new TANK::Tank(glm::vec3(12.0f, 0.0f, 0.0f)); */
  /* engine.getActiveScene().addGameObject(*tank01); */

  Enemy enemy01(glm::vec3(2.0f, 1.0f, 6.0f));
  /* enemy01.rotateByMouse(180.0f * 10.0f); */
  engine.getActiveScene()->addGameObject(enemy01);
  enemies.push_back(&enemy01);

  Enemy enemy02(glm::vec3(6.0f, 1.0f, 20.0f));
  engine.getActiveScene()->addGameObject(enemy02);
  enemies.push_back(&enemy02);

  /* for (int i = 0; i < 20; i++) { */
  /*   Enemy *enemy = new Enemy(glm::vec3(i * 10, 0, 0)); */
  /*   enemies.push_back(enemy); */
  /*   engine.getActiveScene().addGameObject(*enemy); */
  /* } */

  Player player;
  gPlayer = &player;
  engine.getActiveScene()->addGameObject(player);
  engine.getActiveScene()->camera = &player.camera;
  engine.getActiveScene()->mouseCallback = mouseCallback;
  engine.getActiveScene()->renderFrameCallback = processInput;

  // setup lights
  DirectionalLight light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
                         glm::vec3(0.0f, 0.0f, -1.0f));
  glm::vec3 direction = light.direction;
  std::cout << "direction: " << direction.x << ", " << direction.y << ", "
            << direction.z << std::endl;
  engine.getActiveScene()->addLight(&light);

  for (int i = 0; i < 4; i++) {
    PointLight *pointLight = new PointLight();
    pointLight->position = pointLightPositions[i];
    pointLight->ambient = glm::vec3(0.05f);
    pointLight->diffuse = glm::vec3(0.8f);
    pointLight->specular = glm::vec3(1.0f);
    pointLight->constant = 1.0f;
    pointLight->linear = 0.09f;
    pointLight->quadratic = 0.032f;
    engine.getActiveScene()->addLight(pointLight);
  }

  SpotLight spotLight;
  /* spotLight.position = glm::vec3(0.0f, 0.0f, 3.0f); */
  spotLight.ambient = glm::vec3(0.0f);
  spotLight.diffuse = glm::vec3(1.0f);
  spotLight.specular = glm::vec3(1.0f);
  spotLight.constant = 1.0f;
  spotLight.linear = 0.09f;
  spotLight.quadratic = 0.032f;
  spotLight.cutOff = glm::cos(glm::radians(12.5f));
  spotLight.outerCutOff = glm::cos(glm::radians(15.0f));
  engine.getActiveScene()->addLight(&spotLight);

  engine.renderFrameCallback = &renderFrameCallback;

  engine.run();
  return 0;
}

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn) {
  static bool firstMouse = false;
  static float lastX = 0;
  static float lastY = 0;

  float xPos = static_cast<float>(xPosIn);
  float yPos = static_cast<float>(yPosIn);

  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;

  lastX = xPos;
  lastY = yPos;

  if (gPlayer) {
    gPlayer->rotateByMouse(xOffset);
  }
}

void processInput(float deltaTime) {
  GLFWwindow *window = gWindow;
  if (!gPlayer) return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    // player move forward
    gPlayer->move(TANK::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    // player move backward
    gPlayer->move(TANK::BACKWARD, deltaTime);
}
