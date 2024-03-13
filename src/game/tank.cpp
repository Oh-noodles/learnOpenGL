#include "game/tank.hpp"
#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"
#include <string>

using namespace TANK;

Tank::Tank(
    /* std::string const &path, */
    glm::vec3 position,
    glm::vec3 rotation
  ): gameObject(*(new GameObject("resources/objects/simple_tank/scene.gltf", position, rotation, glm::vec3(0.01f)))) {
  this->position = position;
  this->rotation = rotation;
}

void Tank::move(Tank_Direction direction, float deltaTime) {
  float speed = SPEED * deltaTime;
  if (direction == FORWARD)
    gameObject.position += speed * front;
  else if (direction == BACKWARD)
    gameObject.position -= speed * front;
  updateVectors();
}

void Tank::rotate(float xOffset) {
  yaw += xOffset * SENSITIVITY;
  updateVectors();
}

void Tank::updateVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
  direction = glm::normalize(direction);
  front = direction;
}
