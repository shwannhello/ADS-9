// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>
#include <algorithm>

class PMTree {
 private:
  struct Node {
    char value;
    std::vector<std::shared_ptr<Node>> children;

    explicit Node(char val) : value(val) {}
  };

  std::shared_ptr<Node> root;
  std::vector<char> originalElements;

  void buildTree(std::shared_ptr<Node> node, std::vector<char> remaining);
  void collectPerms(std::shared_ptr<Node> node, std::vector<char>& current,
                    std::vector<std::vector<char>>& result);
  void collectPermsSequential(std::shared_ptr<Node> node,
                              std::vector<char>& current, int& counter,
                              int target, std::vector<char>& targetPerm,
                              bool& found);
  void findPermByPath(std::shared_ptr<Node> node, std::vector<char>& current,
                      int& remaining, int target, std::vector<char>& result,
                      bool& found);

 public:
  explicit PMTree(const std::vector<char>& elements);

  friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
  friend std::vector<char> getPerm1(PMTree& tree, int num);
  friend std::vector<char> getPerm2(PMTree& tree, int num);

  int getTotalPermsCount();
  std::shared_ptr<Node> getRoot() const { return root; }
};

#endif  // INCLUDE_TREE_H_
