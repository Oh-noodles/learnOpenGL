#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// default values
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float FOV         = 45.0f;

const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera {
  public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw = YAW;
    float pitch = PITCH;
    float fov = FOV;

    glm::vec3 worldUp;
    // constructor with vectors
    // position, up, yaw, pitch
    Camera(glm::vec3 position, glm::vec3 worldUp, float yaw = YAW, float pitch = PITCH) {
      this->position = position;
      this->worldUp = worldUp;
      this->yaw = yaw;
      this->pitch = pitch;
      this->updateCameraVectors();
    }

    // constructor with scalar values
    // posX, posY, posZ, upX, upY, upZ, yaw, pitch

    // return the view matrix
    glm::mat4 GetViewMatrix() {
      return glm::lookAt(this->position, this->position + this->front, this->up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
      float cameraSpeed = SPEED * deltaTime;
      switch (direction) {
        case FORWARD:
          this->position += cameraSpeed * this->front;
          break;
        case BACKWARD:
          this->position -= cameraSpeed * this->front;
          break;
        case LEFT:
          this->position -= cameraSpeed * this->right;
          break;
        case RIGHT:
          this->position += cameraSpeed * this->right;
          break;
        default:
          break;
      }
      this->updateCameraVectors();
    }

    void ProcessMouseMovement(float xOffset, float yOffset) {
      this->yaw += xOffset * SENSITIVITY;
      this->pitch += yOffset * SENSITIVITY;
      this->pitch = std::min(this->pitch, 89.0f);
      this->pitch = std::max(this->pitch, -89.0f);
      this->updateCameraVectors();
    }

    void ProcessMouseScroll(float yOffset) {
      float fov = this->fov;
      fov -= yOffset;
      fov = std::min(fov, 45.0f);
      fov = std::max(fov, 1.0f);
      this->fov = fov;
    }

  private:
    // calculates the front vector based on Euler Angles
    void updateCameraVectors() {
      glm::vec3 direction;
      direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
      direction.y = sin(glm::radians(this->pitch));
      direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
      direction = glm::normalize(direction);
      this->right = glm::normalize(glm::cross(direction, this->worldUp));
      this->up = glm::normalize(glm::cross(this->right, direction));
      this->front = direction;
    }
};

#endif // !CAMERA_H
