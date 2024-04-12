#include "engine/bvh.hpp"
#include "glm/detail/type_vec.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


Node::Node(bool isLeaf) {
  this->isLeaf = isLeaf;
}

Node::Node(bool isLeaf, float x1, float y1, float z1, float x2, float y2, float z2) {
  this->isLeaf = isLeaf;
  this->x1 = x1; this->y1 = y1; this->z1 = z1;
  this->x2 = x2; this->y2 = y2; this->z2 = z2;
  arrangePositions();
}

Node::Node(bool isLeaf, glm::vec3 lowerPos, glm::vec3 higherPos) {
  this->isLeaf = isLeaf;
  this->x1 = lowerPos.x; this->y1 = lowerPos.y; this->z1 = lowerPos.z;
  this->x2 = higherPos.x; this->y2 = higherPos.y; this->z2 = higherPos.z;
  arrangePositions();
}

Node::Node(bool isLeaf, std::array<float, 6> positions) {
  this->isLeaf = isLeaf;
  this->x1 = positions[0]; this->y1 = positions[1]; this->z1 = positions[2];
  this->x2 = positions[3]; this->y2 = positions[4]; this->z2 = positions[5];
  arrangePositions();
}

void Node::arrangePositions() {
  float x1 = this->x1, y1 = this->y1, z1 = this->z1;
  float x2 = this->x2, y2 = this->y2, z2 = this->z2;
  if (x1 > x2) std::swap(x1, x2);
  if (y1 > y2) std::swap(y1, y2);
  if (z1 > z2) std::swap(z1, z2);
  this->x1 = x1; this->y1 = y1; this->z1 = z1;
  this->x2 = x2; this->y2 = y2; this->z2 = z2;
}

void Node::refit() {
  if (left && right) {
    x1 = std::min(left->x1, right->x1);
    y1 = std::min(left->y1, right->y1);
    z1 = std::min(left->z1, right->z1);
    x2 = std::max(left->x2, right->x2);
    y2 = std::max(left->y2, right->y2);
    z2 = std::max(left->z2, right->z2);
  } else {
    Node *nd = left ? left : right;
    x1 = nd->x1; y1 = nd->y1; z1 = nd->z1;
    x2 = nd->x2; y2 = nd->y2; z2 = nd->z2;
  }
}

float Node::getScore() {
  float dx = abs(x2 - x1);
  float dy = abs(y2 - y1);
  float dz = abs(z2 - z1);
  return 2.0f * (dx*dy + dy*dz + dx*dz);
}

std::array<float, 6> Node::GetCombinedPositions(std::vector<Node *> nodes) {
  bool first = true;
  float xMin, yMin, zMin;
  float xMax, yMax, zMax;
  for (int i = 0; i < nodes.size(); i++) {
    Node *node = nodes[i];
    if (!node) continue;

    if (first) {
      first = false;
      xMin = node->x1; yMin = node->y1; zMin = node->z1;
      xMax = node->x2; yMax = node->y2; zMax = node->z2;
    }

    xMin = std::min(xMin, node->x1);
    yMin = std::min(yMin, node->y1);
    zMin = std::min(zMin, node->z1);
    xMax = std::max(xMax, node->x2);
    yMax = std::max(yMax, node->y2);
    zMax = std::max(zMax, node->z2);
  }

  if (first) assert("all nodes are invalid");

  return {xMin, yMin, zMin, zMax, yMax, zMax};
}

float Node::GetCombinedScore(std::vector<Node *> nodes) {
  bool first = true;
  float x1 = 0, y1 = 0, z1 = 0;
  float x2 = 0, y2 = 0, z2 = 0;
  for (int i = 0; i < nodes.size(); i++) {
    Node *node = nodes[i];
    if (!node) continue;

    if (first) {
      first = false;
      x1 = node->x1; y1 = node->y1; z1 = node->z1;
      x2 = node->x2; y2 = node->y2; z2 = node->z2;
    }

    x1 = std::min(x1, node->x1);
    y1 = std::min(y1, node->y1);
    z1 = std::min(z1, node->z1);
    x2 = std::max(x2, node->x2);
    y2 = std::max(y2, node->y2);
    z2 = std::max(z2, node->z2);
  }
  float dx = x2 - x1;
  float dy = y2 - y1;
  float dz = z2 - z1;
  return 2.0 * (dx*dy + dy*dz + dx*dz);
}

bool Node::TestCollision(Node *node1, Node *node2) {
  if (!node1 || !node2) return false;

  bool collided = !(
      node1->x1 > node2->x2 || node1->x2 < node2->x1 || node1->y1 > node2->y2 || node1->y2 < node2->y1 || node1->z1 > node2->z2 || node1->z2 < node2->z1
      );
  node1->collided |= collided;
  node2->collided |= collided;
  return collided;
}

std::string Node::getSvgPath(int view, std::string color) {
  std::stringstream s;
  float xMin = x1, yMin = y1, zMin = z1;
  float xMax = x2, yMax = y2, zMax = z2;

  if (!isLeaf) {
    // draw the internal node a little bigger
    const float padding = 0.5;
    xMin -= padding;
    yMin -= padding;
    zMin -= padding;
    xMax += padding;
    yMax += padding;
    zMax += padding;
  }

  s << "  <path stroke=\""<< color <<"\" fill-opacity=\"0\" stroke-width=\"0.2\""; 
  s <<" d= \"";
  switch (view) {
    case 0:
      s << "M " << xMin << " " << yMin << " L " << xMax << " " << yMin << " L " << xMax << " " << yMax << " L " << xMin << " " << yMax << " L " << xMin << " " << yMin;
      break;
    case 1:
      s << "M " << xMin << " " << zMin << " L " << xMax << " " << zMin << " L " << xMax << " " << zMax << " L " << xMin << " " << zMax << " L " << xMin << " " << zMin;
      break;
    case 2:
      s << "M " << zMin << " " << yMin << " L " << zMax << " " << yMin << " L " << zMax << " " << yMax << " L " << zMin << " " << yMax << " L " << zMin << " " << yMin;
      break;
  }
  /* s << "M " << x1 << " " << y1 << " L " << x2 << " " << y1 << " L " << x2 << " " << y2 << " L " << x1 << " " << y2 << " L " << x1 << " " << y1; */
  s <<  "\"/>\n";
  return s.str();
}


std::ostream& operator<<(std::ostream &s, Node &node) {
  s << node.x1 << "," << node.y1 << "," << node.x2 << "," << node.y2;
  return s;
}

DTree::DTree() {
  dummyRoot = new Node(false);
}

void DTree::insertLeaf(Node *newLeaf) {
  newLeaf->isLeaf = true;
  // ==== stage 1. finding the best sibling ====
  Node *node = dummyRoot->left;
  if (!node) {
    dummyRoot->left = newLeaf;
    newLeaf->parent = dummyRoot;
    return;
  }

  bool isLeft = true;
  while (!node->isLeaf) {
    // stay, go left, or go right
    float cost = Node::GetCombinedScore({node, newLeaf});

    float inc = cost - node->getScore();
    float costLeft = node->left ? Node::GetCombinedScore({node->left, newLeaf}) + inc : 1.0f + cost;
    float costRight = node->right ? Node::GetCombinedScore({node->right, newLeaf}) + inc : 1.0f + cost;

    if (cost < costLeft && cost < costRight) {
      break;
    }
    if (costLeft < costRight) {
      isLeft = true;
      node = node->left;
    } else {
      isLeft = false;
      node = node->right;
    }
    /* node = costLeft < costRight ? node->left : node->right; */
  }
  // ===========================================

  // stage 2. create new parent and insert
  // node is the sibling we choose
  Node *parent = new Node(false, Node::GetCombinedPositions({node, newLeaf}));
  parent->parent = node->parent;
  if (isLeft) {
    node->parent->left = parent;
  } else {
    node->parent->right = parent;
  }
  parent->left = node;
  parent->right = newLeaf;
  node->parent = parent;
  newLeaf->parent = parent;

  // stage 3. walk back up the tree,
  // reffit, and try to rotate
  /* std::array<float, 4> positions = parent->getPositions(); */
  parent = parent->parent;
  while (parent) {
    parent->refit();

    rotate(parent);

    /* positions = parent->getPositions(); */
    parent = parent->parent;
  }
}

void DTree::testCollision(Node *node, Node *collider) {
  if (Node::TestCollision(node, collider)) {
    if (node->isLeaf) collidedNodes.push_back(node);
    testCollision(node->left, collider);
    testCollision(node->right, collider);
  }
}

std::vector<Node*> DTree::getCollidedNodes(Node *collider) {
  collidedNodes.clear();
  testCollision(dummyRoot->left, collider);
  return collidedNodes;
}

void DTree::rotate(Node *node) {
  if (!node) return;

  Node *left = node->left;
  Node *right = node->right;
  if (!left || !right) return;

  Node *ll = left->left;
  Node *lr = left->right;
  Node *rl = right->left;
  Node *rr = right->right;

  //               A
  //      B(left)      C(right)
  //  D(ll)  E(lr)  F(rl)   G(rr)
  
  //-------------------------------------------
  /* we have these 4 rotate types
  rotate type: 0

          A                A
      B    (C)   =>    B    (D)
  (D)  E  F   G     (C)  E
                  F    G

  SA(B) = DuE    =>   SA(B) = FuGuE
  -------------------------------------------
  rotate type: 1

        A                 A
      B     (C)   =>     B    (E)
  D  (E)  F   G      D  (C)
                        F   G

  SA(B) = DuE    =>   SA(B) = DuFuG
  -------------------------------------------
  rotate type: 2

        A                  A
    (B)    C     =>   (F)      C
  D   E  (F) G              (B)   G
                          D    E

  SA(C) = FuG    =>   SA(C) = DuEuG
  -------------------------------------------
  rotate type: 3

        A                  A
    (B)    C     =>   (G)     C
  D    E F (G)              F   (B)
                              D    E
  SA(C) = FuG    =>   SA(C) = FuDuE
  ------------------------------------------- */
  float scoreB = left->getScore();
  float scoreC = right->getScore();

  // delta scores of all 4 kind of rotations
  float deltaScoreFGE = Node::GetCombinedScore({rl, rr, lr}) - scoreB;
  float deltaScoreDFG = Node::GetCombinedScore({ll, rl, rr}) - scoreB;
  float deltaScoreDEG = Node::GetCombinedScore({ll, lr, rr}) - scoreC;
  float deltaScoreFDE = Node::GetCombinedScore({rl, ll, lr}) - scoreC;

  // pick the rotaion with most negative delta, or do not rotate
  float minDelta = 0;
  int pickIndex = -1;
  std::array<float, 4> scores = {deltaScoreFGE, deltaScoreDFG, deltaScoreDEG, deltaScoreFDE};
  for (int i = 0; i < 4; i++) {
    if (scores[i] < minDelta) {
      minDelta = scores[i];
      pickIndex = i;
    }
  }

  // rotate (or not)
  switch (pickIndex) {
    case -1:
      // do not rotate
      break;
    case 0:
      // rotate type 0
      node->right = ll;
      ll && (ll->parent = node);
      left->left = right;
      right->parent = left;
      left->refit();
      break;
    case 1:
      // rotate type 1
      node->right = lr;
      lr && (lr->parent = node);
      left->right = right;
      right->parent = left;
      left->refit();
      break;
    case 2:
      // rotate type 2
      node->left = rl;
      rl && (rl->parent = node);
      right->left = left;
      left->parent = right;
      right->refit();
      break;
    case 3:
      // rotate type 3
      node->left = rr;
      rr && (rr->parent = node);
      right->right = left;
      left->parent = right;
      right->refit();
      break;
    default:
      break;
  }
}

void DTree::printNode(Node *node, std::string prefix, bool isLeft) {
  if (node == NULL) return;

  std::vector<std::string> leftSymbols = { "├── ", "│   " };
  std::vector<std::string> rightSymbols = { "└── ", "    " };

  std::vector<std::string> symbols = isLeft ? leftSymbols : rightSymbols;

  std::cout << prefix << symbols[0];
  std::cout << (node->isLeaf ? "\033[1;31m" : "\033[1;32m");
  std::cout << node->x1 << "," << node->y1 << "," << node->x2 << "," << node->y2 << std::endl;
  std::cout << "\033[0m";
  printNode(node->left, prefix + symbols[1], true);
  printNode(node->right, prefix + symbols[1], false);
}

void DTree::print() {
  printNode(dummyRoot->left, "", false);
}

void DTree::drawNode(Node *node, std::ofstream &file, int view) {
  if (!node) return;

  std::string color = node->isLeaf ?
    node->collided ? "blue" : "red"
    : "green";
  file << node->getSvgPath(view, color);
  drawNode(node->left, file, view);
  drawNode(node->right, file, view);
}

void DTree::drawSvg(int view) {
  std::string filename = view == 0 ? "output-front.svg" : view == 1 ? "output-top.svg" : "output-left.svg";
  std::ofstream file;
  file.open(filename);
  file << "<svg width=\"1000\" height=\"1000\" viewBox=\"0 0 150 150\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  file.close();
  file.open(filename, std::ios_base::app);
  drawNode(dummyRoot->left, file, view);
  drawNode(dummyRoot->right, file, view);
  file << "</svg>\n";
  file.close();
}
