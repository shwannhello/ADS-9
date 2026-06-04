// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;
using namespace chrono;

template<typename Func>
double measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000.0;
}

int getRandomPermNumber(int maxNum) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxNum);
    return dis(gen);
}

void runExperiment(int n, ofstream& dataFile) {
    vector<char> elements;
    for (int i = 0; i < n; ++i) {
        elements.push_back('0' + (i % 10));
    }
    
    cout << "Testing n = " << n << " (permutations: ";
    long long perms = 1;
    for (int i = 2; i <= n; ++i) perms *= i;
    cout << perms << ")" << endl;
    
    auto buildStart = high_resolution_clock::now();
    PMTree tree(elements);
    auto buildEnd = high_resolution_clock::now();
    double buildTime = duration_cast<microseconds>(buildEnd - buildStart).count() / 1000.0;
    cout << "  Tree built in " << buildTime << " ms" << endl;
    
    vector<int> testNumbers;
    for (int i = 0; i < 10; ++i) {
        testNumbers.push_back(getRandomPermNumber(perms));
    }
    
    double getAllTime = 0.0;
    if (n <= 8) {
        getAllTime = measureTime([&]() {
            auto perms = getAllPerms(tree);
        });
        cout << "  getAllPerms: " << getAllTime << " ms" << endl;
    } else {
        getAllTime = -1;
        cout << "  getAllPerms: skipped (too large)" << endl;
    }
    
    double perm1Time = 0.0;
    for (int num : testNumbers) {
        perm1Time += measureTime([&]() {
            auto result = getPerm1(tree, num);
        });
    }
    perm1Time /= 10.0;
    cout << "  getPerm1 (avg of 10 random): " << perm1Time << " ms" << endl;
    
    double perm2Time = 0.0;
    for (int num : testNumbers) {
        perm2Time += measureTime([&]() {
            auto result = getPerm2(tree, num);
        });
    }
    perm2Time /= 10.0;
    cout << "  getPerm2 (avg of 10 random): " << perm2Time << " ms" << endl;
    
    dataFile << n << "\t" << perms << "\t" 
             << (getAllTime > 0 ? getAllTime : -1) << "\t"
             << perm1Time << "\t" << perm2Time << "\t"
             << buildTime << endl;
    
    cout << endl;
}

int main() {
    cout << "=== Permutation Tree Generator ===" << endl << endl;
    
    cout << "Examples:" << endl;
    vector<char> example = {'1', '2', '3'};
    PMTree tree(example);
    
    cout << "Tree built for elements: ";
    for (char c : example) cout << c << " ";
    cout << endl << endl;
    
    auto allPerms = getAllPerms(tree);
    cout << "All permutations:" << endl;
    for (size_t i = 0; i < allPerms.size(); ++i) {
        cout << i + 1 << ": ";
        for (char c : allPerms[i]) cout << c;
        cout << endl;
    }
    cout << endl;
    
    for (int num = 1; num <= 6; ++num) {
        auto perm1 = getPerm1(tree, num);
        auto perm2 = getPerm2(tree, num);
        
        cout << "Permutation #" << num << ": ";
        cout << "getPerm1: ";
        for (char c : perm1) cout << c;
        cout << ", getPerm2: ";
        for (char c : perm2) cout << c;
        cout << endl;
    }
    cout << endl;
    
    auto invalid = getPerm1(tree, 10);
    cout << "Permutation #10 (does not exist): " 
         << (invalid.empty() ? "empty vector" : "not empty") << endl << endl;
    
    cout << "=== EXPERIMENT ===" << endl << endl;
    
    ofstream resultsFile("results.txt");
    resultsFile << "n\tnum_permutations\tgetAllPerms_ms\tgetPerm1_ms\tgetPerm2_ms\tbuildTime_ms" << endl;
    
    vector<int> testSizes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (int n : testSizes) {
        runExperiment(n, resultsFile);
    }
    
    resultsFile.close();
    
    cout << "\nExperiment complete. Results saved to results.txt" << endl;
    cout << "You can now create graph manually from results.txt" << endl;
    return 0;
}
int main() {
  return 0;
}
