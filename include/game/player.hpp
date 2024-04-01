#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "game/tank.hpp"
#include "glm/detail/type_vec.hpp"
#include "learn/camera.hpp"

class Player: public TANK::Tank {
  private:
    void updateCamera();

  public:
    Camera &camera;
    bool collided = false;
    Player(
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f)
    );
    void move(TANK::Tank_Direction direction, float deltaTime);
    void rotateByMouse(float xOffset);

    void renderFrameCallback(float deltaTime);
};

#endif // !DEBUG
