// Copyright 2022 NNTU-CS
#include "tree.h"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

template<typename Func>
double measureTime(Func func) {
  auto start = high_resolution_clock::now();
  func();
  auto end = high_resolution_clock::now();
  return duration_cast<microseconds>(end - start).count() / 1000.0;
}

int getRandomPermNumber(int maxNum) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, maxNum);
  return dis(gen);
}

void runExperiment(int n, std::ofstream& dataFile) {
  std::vector<char> elements;
  for (int i = 0; i < n; ++i) {
    elements.push_back('0' + (i % 10));
  }

  std::cout << "Testing n = " << n << " (permutations: ";
  int64_t perms = 1;
  for (int i = 2; i <= n; ++i) perms *= i;
  std::cout << perms << ")" << std::endl;

  auto buildStart = high_resolution_clock::now();
  PMTree tree(elements);
  auto buildEnd = high_resolution_clock::now();
  double buildTime = duration_cast<microseconds>(buildEnd - buildStart).count()
                     / 1000.0;
  std::cout << "  Tree built in " << buildTime << " ms" << std::endl;

  std::vector<int> testNumbers;
  for (int i = 0; i < 10; ++i) {
    testNumbers.push_back(getRandomPermNumber(perms));
  }

  double getAllTime = 0.0;
  if (n <= 8) {
    getAllTime = measureTime([&]() {
      auto permsResult = getAllPerms(tree);
    });
    std::cout << "  getAllPerms: " << getAllTime << " ms" << std::endl;
  } else {
    getAllTime = -1;
    std::cout << "  getAllPerms: skipped (too large)" << std::endl;
  }

  double perm1Time = 0.0;
  for (int num : testNumbers) {
    perm1Time += measureTime([&]() {
      auto result = getPerm1(tree, num);
    });
  }
  perm1Time /= 10.0;
  std::cout << "  getPerm1 (avg of 10 random): " << perm1Time << " ms"
            << std::endl;

  double perm2Time = 0.0;
  for (int num : testNumbers) {
    perm2Time += measureTime([&]() {
      auto result = getPerm2(tree, num);
    });
  }
  perm2Time /= 10.0;
  std::cout << "  getPerm2 (avg of 10 random): " << perm2Time << " ms"
            << std::endl;

  dataFile << n << "\t" << perms << "\t"
           << (getAllTime > 0 ? getAllTime : -1) << "\t"
           << perm1Time << "\t" << perm2Time << "\t"
           << buildTime << std::endl;

  std::cout << std::endl;
}

int main() {
  std::cout << "=== Permutation Tree Generator ===" << std::endl << std::endl;

  std::cout << "Examples:" << std::endl;
  std::vector<char> example = {'1', '2', '3'};
  PMTree tree(example);

  std::cout << "Tree built for elements: ";
  for (char c : example) std::cout << c << " ";
  std::cout << std::endl << std::endl;

  auto allPerms = getAllPerms(tree);
  std::cout << "All permutations:" << std::endl;
  for (size_t i = 0; i < allPerms.size(); ++i) {
    std::cout << i + 1 << ": ";
    for (char c : allPerms[i]) std::cout << c;
    std::cout << std::endl;
  }
  std::cout << std::endl;

  for (int num = 1; num <= 6; ++num) {
    auto perm1 = getPerm1(tree, num);
    auto perm2 = getPerm2(tree, num);

    std::cout << "Permutation #" << num << ": ";
    std::cout << "getPerm1: ";
    for (char c : perm1) std::cout << c;
    std::cout << ", getPerm2: ";
    for (char c : perm2) std::cout << c;
    std::cout << std::endl;
  }
  std::cout << std::endl;

  auto invalid = getPerm1(tree, 10);
  std::cout << "Permutation #10 (does not exist): "
            << (invalid.empty() ? "empty vector" : "not empty") << std::endl
            << std::endl;

  std::cout << "=== EXPERIMENT ===" << std::endl << std::endl;

  std::ofstream resultsFile("results.txt");
  resultsFile << "n\tnum_permutations\tgetAllPerms_ms\tgetPerm1_ms\t"
              << "getPerm2_ms\tbuildTime_ms" << std::endl;

  std::vector<int> testSizes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int n : testSizes) {
    runExperiment(n, resultsFile);
  }

  resultsFile.close();

  std::cout << "\nExperiment complete. Results saved to results.txt"
            << std::endl;
  std::cout << "You can now create graph manually from results.txt"
            << std::endl;

  return 0;
}
