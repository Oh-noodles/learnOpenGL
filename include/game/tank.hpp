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
};

const float YAW         = 90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;

class Tank {
  private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 front;

    float yaw = YAW;
    float pitch = PITCH;

    void updateVectors();

  public:
    GameObject &gameObject;
    Tank(
      /* std::string const &path, */
      glm::vec3 position = glm::vec3(0.0f),
      glm::vec3 rotation = glm::vec3(0.0f)
    );
    void move(Tank_Direction direction, float deltaTime);
    void rotate(float xOffset);
};

}

#endif // !__TANK_HPP__
