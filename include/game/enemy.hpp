#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "game/tank.hpp"
#include "glm/detail/type_vec.hpp"

class Enemy: public TANK::Tank {
  private:
    TANK::Tank_Direction drivingDirection;
    float drivingCountdown = 0;

  public:
    bool collided = false;
    Enemy(
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f)
    );

    void renderFrameCallback(float deltaTime);
    void autoDrive(float deltaTime);
};

#endif // !DEBUG
