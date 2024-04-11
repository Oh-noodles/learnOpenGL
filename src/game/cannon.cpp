#include "game/cannon.hpp"
#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"

using namespace CANNON;

Cannon::Cannon(glm::vec3 position, glm::vec3 direction):
  GameObject("resources/objects/cannon/cannon.glb", position)
{
  this->direction = direction;
}

void Cannon::update(float deltaTime) {
  // update position
  position += direction * SPEED * deltaTime;
  // apply gravity
  position.y -= GRAVITY * deltaTime;

  if (position.y < - GRAVITY * deltaTime * 60 * 10) {
    // destroy cannon under ground
    destroy();
  }
}

void Cannon::renderFrameCallback(float deltaTime) {
  if (toDestroy) return;
  /* std::cout << "cannon renderFrameCallback" << std::endl; */
  update(deltaTime);
}
