#include "engine/engine.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  GLFWwindow *window = Engine::createWindow(SCR_WIDTH, SCR_HEIGHT);
  Engine engine(window, SCR_WIDTH, SCR_HEIGHT);
  engine.addScene();
  engine.addModel("resources/objects/backpack/backpack.obj", -2.0f, 0.0f, 0.0f);
  engine.addModel("resources/objects/backpack/backpack.obj", 2.0f, 0.0f, 0.0f);
  engine.run();
  return 0;
}
