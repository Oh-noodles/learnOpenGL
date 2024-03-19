#include "game/enemy.hpp"
#include "game/tank.hpp"
#include "glm/detail/type_vec.hpp"
#include <ctime>
#include <iostream>

Enemy::Enemy(glm::vec3 position, glm::vec3 rotation): TANK::Tank(position, rotation) {}

void Enemy::renderFrameCallback(float deltaTime) {
  /* std::cout << "enemy renderFrameCallback: " << std::time(nullptr)<< std::endl; */
  autoDrive(deltaTime);
  /* move(TANK::FORWARD, deltaTime); */
}

void Enemy::autoDrive(float deltaTime) {
  if (drivingCountdown <= 0) {
    std::srand(std::time(nullptr) * std::rand());
    TANK::Tank_Direction direction = (TANK::Tank_Direction)(std::rand() % 4);
    drivingDirection = direction;
    drivingCountdown = std::rand() % (drivingDirection < 2 ? 10 : 4);
    std::cout << "direction: " << direction << std::endl;
  }
  /* std::cout << "drivingCountdown: " << drivingCountdown << std::endl; */

  switch (drivingDirection) {
    case TANK::FORWARD:
    case TANK::BACKWARD:
      move(drivingDirection, TANK::SPEED * deltaTime);
      break;
    case TANK::LEFT:
    case TANK::RIGHT:
      rotate(drivingDirection, deltaTime);
      break;
    default:
      break;
  }
  drivingCountdown -= deltaTime;
}
