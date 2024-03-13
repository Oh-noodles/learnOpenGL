#include "engine/engine.hpp"
#include "engine/scene.hpp"
#include "game/tank.hpp"
#include "glm/detail/func_trigonometric.hpp"
#include "glm/detail/type_vec.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

TANK::Tank *tank01;

void renderFrameCallback(float deltaTime) {
  tank01->move(TANK::FORWARD, 2.5f * deltaTime);
}

int main() {
  glm::vec3 pointLightPositions[] = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, -3.0f)
  };

  GLFWwindow *window = Engine::createWindow(SCR_WIDTH, SCR_HEIGHT);
  Engine engine(window, SCR_WIDTH, SCR_HEIGHT);
  engine.addScene();
  engine.getActiveScene().addGameObject("resources/objects/backpack/backpack.obj", glm::vec3(-6.0f, 0.0f, 0.0f));
  engine.getActiveScene().addGameObject("resources/objects/simple_tank/scene.gltf", glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.01f));

  tank01 = new TANK::Tank(glm::vec3(12.0f, 0.0f, 0.0f));
  engine.getActiveScene().addGameObject(tank01->gameObject);

  // setup lights
  DirectionalLight light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  glm::vec3 direction = light.direction;
  std::cout << "direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
  engine.getActiveScene().addLight(&light);

  for (int i = 0; i < 4; i++) {
    PointLight *pointLight = new PointLight();
    pointLight->position = pointLightPositions[i];
    pointLight->ambient = glm::vec3(0.05f);
    pointLight->diffuse = glm::vec3(0.8f);
    pointLight->specular = glm::vec3(1.0f);
    pointLight->constant = 1.0f;
    pointLight->linear = 0.09f;
    pointLight->quadratic = 0.032f;
    engine.getActiveScene().addLight(pointLight);
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
  engine.getActiveScene().addLight(&spotLight);

  engine.renderFrameCallback = &renderFrameCallback;

  engine.run();
  return 0;
}
