#include "game/tank.hpp"
#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <iostream>
#include <string>

using namespace TANK;

void callback(float deltaTime) {}

Tank::Tank(
    /* std::string const &path, */
    glm::vec3 position,
    glm::vec3 rotation
  ): GameObject("resources/objects/simple_tank/scene.gltf", position, rotation, glm::vec3(0.01)) {
  addCollider(glm::vec3(-1.0f), glm::vec3(1.0f));
}

void Tank::move(Tank_Direction direction, float deltaTime) {
  float speed = SPEED * deltaTime;
  if (direction == FORWARD)
    position += speed * front;
  else if (direction == BACKWARD)
    position -= speed * front;
  updateVectors();
}

void Tank::rotate(Tank_Direction direction, float deltaTime) {
  yaw += (direction == LEFT ? -1 : 1) * ROTSPEED * deltaTime;
  updateVectors();
}

void Tank::rotateByMouse(float xOffset) {
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
  float angle = glm::orientedAngle(direction, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  rotation = glm::vec3(0.0f, angle, 0.0f);
}


void Tank::renderFrameCallback(float deltaTime) {
  /* std::cout << "tank renderFrameCallback: " << deltaTime << std::endl; */
  collider->update(position);
}
