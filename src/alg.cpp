// src/alg.cpp
// Copyright 2024 <Student>

#include "../include/alg.h"
#include "../include/tree.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

PMTree::PMTree(const std::vector<char>& elements)
    : originalElements(elements) {
  if (elements.empty()) {
    root = nullptr;
    return;
  }

  std::vector<char> sorted = elements;
  std::sort(sorted.begin(), sorted.end());

  root = std::make_shared<Node>('\0');
  buildTree(root, sorted);
}

void PMTree::buildTree(std::shared_ptr<Node> node,
                       std::vector<char> remaining) {
  if (remaining.empty()) {
    return;
  }

  for (size_t i = 0; i < remaining.size(); ++i) {
    char val = remaining[i];
    auto child = std::make_shared<Node>(val);
    node->children.push_back(child);

    std::vector<char> newRemaining;
    for (size_t j = 0; j < remaining.size(); ++j) {
      if (j != i) {
        newRemaining.push_back(remaining[j]);
      }
    }

    buildTree(child, newRemaining);
  }
}

void PMTree::collectPerms(std::shared_ptr<Node> node,
                          std::vector<char>& current,
                          std::vector<std::vector<char>>& result) {
  if (!node || node->children.empty()) {
    if (!current.empty()) {
      result.push_back(current);
    }
    return;
  }

  for (auto& child : node->children) {
    current.push_back(child->value);
    collectPerms(child, current, result);
    current.pop_back();
  }
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> result;
  if (!tree.root) return result;

  std::vector<char> current;
  tree.collectPerms(tree.root, current, result);
  return result;
}

void PMTree::collectPermsSequential(std::shared_ptr<Node> node,
                                    std::vector<char>& current, int& counter,
                                    int target, std::vector<char>& targetPerm,
                                    bool& found) {
  if (found) return;

  if (!node || node->children.empty()) {
    if (!current.empty()) {
      counter++;
      if (counter == target) {
        targetPerm = current;
        found = true;
      }
    }
    return;
  }

  for (auto& child : node->children) {
    if (found) return;
    current.push_back(child->value);
    collectPermsSequential(child, current, counter, target,
                           targetPerm, found);
    current.pop_back();
  }
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  std::vector<char> result;
  if (!tree.root || num < 1) return result;

  int totalPerms = 1;
  for (int i = 2; i <= static_cast<int>(tree.originalElements.size());
       ++i) {
    totalPerms *= i;
  }

  if (num > totalPerms) return result;

  int counter = 0;
  std::vector<char> current;
  bool found = false;

  tree.collectPermsSequential(tree.root, current, counter, num,
                              result, found);

  return result;
}

void PMTree::findPermByPath(std::shared_ptr<Node> node,
                            std::vector<char>& current, int& remaining,
                            int target, std::vector<char>& result,
                            bool& found) {
  if (found) return;

  if (!node || node->children.empty()) {
    if (!current.empty()) {
      result = current;
      found = true;
    }
    return;
  }

  int permsPerChild = 1;
  for (int i = 1; i <= remaining - 1; ++i) {
    permsPerChild *= i;
  }

  int childIndex = (target - 1) / permsPerChild;

  if (childIndex < static_cast<int>(node->children.size())) {
    current.push_back(node->children[childIndex]->value);
    int newTarget = target - childIndex * permsPerChild;
    remaining--;
    findPermByPath(node->children[childIndex], current, remaining,
                   newTarget, result, found);
  }
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  std::vector<char> result;
  if (!tree.root || num < 1) return result;

  int totalPerms = 1;
  for (int i = 2; i <= static_cast<int>(tree.originalElements.size());
       ++i) {
    totalPerms *= i;
  }

  if (num > totalPerms) return result;

  std::vector<char> current;
  int remaining = static_cast<int>(tree.originalElements.size());
  bool found = false;

  tree.findPermByPath(tree.root, current, remaining, num, result, found);

  return result;
}

int PMTree::getTotalPermsCount() {
  int count = 1;
  for (int i = 2; i <= static_cast<int>(originalElements.size()); ++i) {
    count *= i;
  }
  return count;
}
