#ifndef __TANK_HPP__
#define __TANK_HPP__

#include "engine/gameObject.hpp"
#include "glm/glm.hpp"
#include "glm/detail/type_vec.hpp"
#include <string>

namespace TANK {

enum Tank_Direction {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const float YAW         = 90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 1.0f;
const float ROTSPEED    = 30.0f;
const float SENSITIVITY = 0.1f;

class Tank: public GameObject {
  private:
    /* glm::vec3 position; */
    /* glm::vec3 rotation; */

    float yaw = YAW;
    float pitch = PITCH;

    void updateVectors();

  public:
    glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
    /* GameObject &gameObject; */
    Tank(
      /* std::string const &path, */
      glm::vec3 position = glm::vec3(0.0f),
      glm::vec3 rotation = glm::normalize(glm::vec3(0.0f))
    );
    void move(Tank_Direction direction, float deltaTime);
    void rotate(Tank_Direction direction, float deltaTime);
    void rotateByMouse(float xOffset);

    virtual void renderFrameCallback(float deltaTime);
};

}

#endif // !__TANK_HPP__
