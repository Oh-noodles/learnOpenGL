#include "engine/collider.hpp"
#include "engine/bvh.hpp"
#include "engine/utils.hpp"
#include "glm/detail/type_vec.hpp"
#include <array>

DTree* Collider::dTree = new DTree();

Collider::Collider(glm::vec3 position, glm::vec3 lowOffset, glm::vec3 highOffset): 
  id(Utils::uuid()), 
  lowOffset(lowOffset), 
  highOffset(highOffset) {
    Collider::dTree->insertLeaf(new Node(id, true, getColliderPositions(position)));
}

std::array<float, 6> Collider::getColliderPositions(glm::vec3 position) {
  glm::vec3 lowPos = position + lowOffset;
  glm::vec3 highPos = position + highOffset;
  return {lowPos.x, lowPos.y, lowPos.z, highPos.x, highPos.y, highPos.z};
}

void Collider::update(glm::vec3 position) {
  Collider::dTree->updateLeaf(id, getColliderPositions(position));
}
