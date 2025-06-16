#include "engine/bvh.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int randint(int mn = 0, int mx = 100) {
  std::srand(std::time(nullptr) * std::rand());
  int val = abs(std::rand());
  val = mn + (val % (mx - mn));
  return val;
} 


int main() {
  int width = 6;
  DTree tree;
  std::vector<Node*> nodesToRemove = {};
  std::vector<Node*> nodesToUpdate = {};

  for (int i = 0; i < 6; i++) {
    int x1 = i * 10; // randint();
    int y1 = 0; // randint();
    int z1 = 0; // randint();
    Node *node = new Node(std::to_string(i), true, x1, y1, z1, x1+width, y1+width, z1+width);
    /* if (x1 >= 30) */
    /*   nodesToRemove.push_back(node); */
    /* if (y1 > 50) */
    /*   nodesToUpdate.push_back(node); */
    tree.insertLeaf(node);
    std::vector<Node*> collided = tree.getCollidedNodes(node);
    if (collided.size() > 0) {
      node->collided = true;
    }
  }


  std::cout << "nodesToRemove: " << nodesToRemove.size() << std::endl;
  for (int i = 0; i < nodesToRemove.size(); i++) {
    Node *node = nodesToRemove[i];
    tree.removeLeaf(node);
  }

  for (int i = 0; i < nodesToUpdate.size(); i++) {
    Node *node = nodesToUpdate[i];
    int x1 = randint();
    int y1 = randint();
    int z1 = randint();
    tree.updateLeaf(node->id, x1, y1, z1, x1+width, y1+width, z1+width);
  }

  Node *collider = new Node("", true, randint(), randint(), randint(), randint(), randint(), randint());
  collider->collided = true;
  tree.dummyRoot->right = collider;
  std::cout << "collider: " << *collider << std::endl;

  std::vector<Node *> collidedNodes = tree.getCollidedNodes(collider);

  tree.print();

  tree.drawSvg(0);
  tree.drawSvg(1);
  tree.drawSvg(2);
  return 0;
}
