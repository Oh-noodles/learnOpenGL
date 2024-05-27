#include "game/player.hpp"
#include "engine/collider.hpp"
#include "game/tank.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/camera.hpp"

Player::Player(glm::vec3 position, glm::vec3 rotation): TANK::Tank(position, rotation) {
  /* camera(*(new Camera(position)))  */
  camera = new Camera(position);
  camera->position.y += 4;
}

void Player::move(TANK::Tank_Direction direction, float deltaTime) {
  // move tank
  TANK::Tank::move(direction, deltaTime);
  // and update camera
  updateCamera();
}

void Player::rotateByMouse(float xOffset) {
  // rotate tank
  TANK::Tank::rotateByMouse(xOffset);
  // and update camera
  updateCamera();
}

void Player::updateCamera() {
  glm::vec3 pos = this->position;
  pos.y = camera->position.y;
  camera->position = pos;
  camera->yaw = this->yaw;
  camera->pitch = this->pitch;
  camera->updateCameraVectors();
}

void Player::renderFrameCallback(float deltaTime) {
  collider->update(position);
  Collider::dTree->drawSvg(1);
}
