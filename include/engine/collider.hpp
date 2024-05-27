#ifndef __COLLIDER_HPP__
#define __COLLIDER_HPP__

#include "engine/bvh.hpp"
#include "glm/glm.hpp"
#include "glm/detail/type_vec.hpp"
#include <array>
#include <string>
class Collider {
  public:
    static DTree *dTree;
    std::string id;
    glm::vec3 lowOffset;
    glm::vec3 highOffset;

    Collider(glm::vec3 position, glm::vec3 lowOffset, glm::vec3 highOffset);
    std::array<float, 6> getColliderPositions(glm::vec3 position);
    void update(glm::vec3 position);
};

#endif // !__COLLIDER_HPP__
#define __COLLIDER_HPP__

