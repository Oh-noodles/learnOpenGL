#ifndef __CANNON_HPP__
#define __CANNON_HPP__

#include "engine/gameObject.hpp"
#include "glm/detail/type_vec.hpp"

namespace CANNON {

  const float SPEED   = 60.0f;
  const float GRAVITY = 6.0f;

  class Cannon: public GameObject {
    private:
      glm::vec3 direction;
      void update(float deltaTime);

    public:
      Cannon(glm::vec3 position, glm::vec3 direction);
      virtual void renderFrameCallback(float deltaTime);
  };

}

#endif // !DEBUG
