// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>
#include <algorithm>

class TreeNode {
 public:
    explicit TreeNode(char val) : value(val) {}
    char value;
    std::vector<std::shared_ptr<TreeNode>> children;
};

class PMTree {
 private:
    std::shared_ptr<TreeNode> root;
    std::vector<char> originalElements;

    void buildTree(std::shared_ptr<TreeNode> node, std::vector<char> remaining);
    void getAllPermsRecursive(std::shared_ptr<TreeNode> node,
                              std::vector<char>& current,
                              std::vector<std::vector<char>>& result);

 public:
    explicit PMTree(const std::vector<char>& elements);
    ~PMTree() = default;

    std::shared_ptr<TreeNode> getRoot() const { return root; }
    friend std::vector<std::vector<char>> getAllPerms(PMTree& tree);
    friend std::vector<char> getPerm1(PMTree& tree, int num);
    friend std::vector<char> getPerm2(PMTree& tree, int num);
};

// Функции для работы с деревом
std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

// Вспомогательная функция для вычисления факториала
int factorial(int n);


#endif  // INCLUDE_TREE_H_
