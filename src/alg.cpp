// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include <memory>
#include  "tree.h"

// Вычисление факториала
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Конструктор дерева
PMTree::PMTree(const std::vector<char>& elements) : originalElements(elements) {
    if (elements.empty()) {
        root = nullptr;
        return;
    }

    root = std::make_shared<TreeNode>('\0');  // Корневой узел-заглушка
    std::vector<char> sorted = elements;
    std::sort(sorted.begin(), sorted.end());
    buildTree(root, sorted);
}

// Построение дерева рекурсивно
void PMTree::buildTree(std::shared_ptr<TreeNode> node,
                       std::vector<char> remaining) {
    if (remaining.empty()) {
        return;
    }

    // Создаем потомков для каждого оставшегося символа
    for (size_t i = 0; i < remaining.size(); ++i) {
        char val = remaining[i];
        auto child = std::make_shared<TreeNode>(val);
        node->children.push_back(child);

        // Создаем вектор оставшихся символов (исключая текущий)
        std::vector<char> newRemaining;
        for (size_t j = 0; j < remaining.size(); ++j) {
            if (j != i) {
                newRemaining.push_back(remaining[j]);
            }
        }

        // Рекурсивно строим поддерево
        buildTree(child, newRemaining);
    }
}

// Рекурсивный обход для получения всех перестановок
void PMTree::getAllPermsRecursive(std::shared_ptr<TreeNode> node,
                                  std::vector<char>& current,
                                  std::vector<std::vector<char>>& result) {
    if (!node || node->value == '\0') {
        return;
    }

    current.push_back(node->value);

    if (node->children.empty()) {
        result.push_back(current);
    } else {
        // Обход детей слева направо для сохранения порядка по возрастанию
        for (auto& child : node->children) {
            getAllPermsRecursive(child, current, result);
        }
    }

    current.pop_back();
}

// Функция получения всех перестановок
std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
    std::vector<std::vector<char>> result;

    if (!tree.getRoot() || tree.getRoot()->children.empty()) {
        return result;
    }

    std::vector<char> current;

    // Обходим всех детей корня
    for (auto& child : tree.getRoot()->children) {
        tree.getAllPermsRecursive(child, current, result);
    }

    return result;
}

// Первый способ: предварительный обход всех предшествующих перестановок
std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num < 1) return std::vector<char>();

    std::vector<std::vector<char>> allPerms = getAllPerms(tree);

    if (num > static_cast<int>(allPerms.size())) {
        return std::vector<char>();
    }

    return allPerms[num - 1];
}

// Второй способ: прямая навигация по дереву
std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num < 1) return std::vector<char>();

    std::vector<char> result;
    std::shared_ptr<TreeNode> currentNode = tree.getRoot();

    if (!currentNode || currentNode->children.empty()) {
        return result;
    }

    int remaining = num - 1;  // переходим к 0-индексации

    // Пока не достигли листа
    while (currentNode && !currentNode->children.empty()) {
        size_t numChildren = currentNode->children.size();

        // Вычисляем размер поддерева для каждого потомка
        int subTreeSize = factorial(static_cast<int>(numChildren - 1));

        // Определяем индекс нужного потомка
        int childIndex = remaining / subTreeSize;

        if (childIndex >= static_cast<int>(numChildren)) {
            return std::vector<char>();  // Некорректный номер
        }

        // Переходим к выбранному потомку
        currentNode = currentNode->children[childIndex];
        result.push_back(currentNode->value);

        // Обновляем remaining для следующего уровня
        remaining = remaining % subTreeSize;
    }

    return result;
}
