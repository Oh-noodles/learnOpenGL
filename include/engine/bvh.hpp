#ifndef __BVH_HPP__
#define __BVH_HPP__

#include "glm/glm.hpp"
#include "glm/detail/type_vec.hpp"
#include <array>
#include <fstream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
class Node {
  private:
    void arrangePositions();
    std::string genId();
  public:
    std::string id;
    float x1 = 0, y1 = 0, z1 = 0;
    float x2 = 0, y2 = 0, z2 = 0;
    /* glm::vec3 lowerPos = glm::vec3(0.0f); */
    /* glm::vec3 higherPos = glm::vec3(0.0f); */
    Node *parent = NULL;
    Node *left = NULL;
    Node *right = NULL;
    bool isLeaf = true;
    bool collided = false;

    static float GetCombinedScore(std::vector<Node*> nodes);
    static std::array<float, 6> GetCombinedPositions(std::vector<Node*> nodes);
    static bool TestCollision(Node *node1, Node *node2);

    Node(std::string id, bool isLeaf);
    Node(std::string id, bool isLeaf,
        float x1, float y1, float z1,
        float x2, float y2, float z2);
    Node(std::string id, bool isLeaf, glm::vec3 lowerPos, glm::vec3 higherPos);
    Node(std::string id, bool isLeaf, std::array<float, 6>);

    /* void updatePositions(float x1, float y1, float z1, float x2, float y2, float z2); */
    float getScore();
    // when the child node changed, we should refit the parent node
    void refit();
    // view: front(0), top(1), left(2)
    std::string getSvgPath(int view = 0, std::string color = "black");
};

std::ostream& operator<<(std::ostream &s, Node &node);

class DTree {
  private:
    std::map<std::string, Node*> nodeMap = {};
    void rotate(Node *node);
    void printNode(Node *node, std::string prefix, bool isLeft);
    void testCollision(Node *node, Node *collider);
    void drawNode(Node *node, std::ofstream &file, int view);
  public:
    Node *dummyRoot;
    std::vector<Node*> collidedNodes = {};
    DTree();

    void insertLeaf(Node *newLeaf);
    void removeLeaf(Node *rmLeaf);
    void removeLeaf(std::string id);
    void updateLeaf(Node *updLeaf,
        float x1, float y1, float z1,
        float x2, float y2, float z2);
    void updateLeaf(std::string id,
        float x1, float y1, float z1,
        float x2, float y2, float z2);
    std::vector<Node*> getCollidedNodes(Node *collider);
    void print();
    // view: front(0), top(1), left(2)
    void drawSvg(int view = 0);
};

#endif // !__BVH_HPP__
