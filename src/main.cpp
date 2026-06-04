// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

// Функция для измерения времени выполнения
template<typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

// Функция для генерации случайного номера перестановки
int getRandomPermNumber(int maxNum, mt19937& gen) {
    uniform_int_distribution<> dis(1, maxNum);
    return dis(gen);
}

// Функция для проведения эксперимента
void runExperiment() {
    cout << "\n=== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ===" << endl;
    cout << "Измерение времени выполнения функций в зависимости от n\n" << endl;
    
    vector<int> sizes = {3, 4, 5, 6, 7, 8};
    vector<long long> timesGetAllPerms, timesGetPerm1, timesGetPerm2;
    
    random_device rd;
    mt19937 gen(rd());
    
    cout << setw(5) << "n" 
         << setw(20) << "getAllPerms (мкс)"
         << setw(20) << "getPerm1 (мкс)"
         << setw(20) << "getPerm2 (мкс)" << endl;
    cout << string(65, '-') << endl;
    
    for (int n : sizes) {
        // Создаем вектор символов
        vector<char> elements;
        for (int i = 0; i < n; ++i) {
            elements.push_back('0' + (i % 10));
        }
        
        // Строим дерево
        PMTree tree(elements);
        
        // Генерируем случайный номер перестановки
        int totalPerms = factorial(n);
        int randomNum = getRandomPermNumber(totalPerms, gen);
        
        // Измеряем время getAllPerms
        long long timeAllPerms = measureTime([&]() {
            getAllPerms(tree);
        });
        timesGetAllPerms.push_back(timeAllPerms);
        
        // Измеряем время getPerm1
        long long timePerm1 = measureTime([&]() {
            getPerm1(tree, randomNum);
        });
        timesGetPerm1.push_back(timePerm1);
        
        // Измеряем время getPerm2
        long long timePerm2 = measureTime([&]() {
            getPerm2(tree, randomNum);
        });
        timesGetPerm2.push_back(timePerm2);
        
        // Выводим результаты
        cout << setw(5) << n 
             << setw(20) << timeAllPerms
             << setw(20) << timePerm1
             << setw(20) << timePerm2 << endl;
    }
    
    cout << "\nПримечание: getPerm1 использует getAllPerms, поэтому их время выполнения相近о." << endl;
    cout << "getPerm2 работает значительно быстрее, так как использует прямую навигацию по дереву." << endl;
}

// Функция демонстрации работы функций
void demonstrateFunctions() {
    cout << "\n=== ДЕМОНСТРАЦИЯ РАБОТЫ ФУНКЦИЙ ===" << endl;
    
    // Пример 1: перестановки для {'1','2','3'}
    vector<char> in1 = {'1', '2', '3'};
    PMTree tree1(in1);
    
    cout << "\n1. Для набора символов: ";
    for (char c : in1) cout << c << " ";
    cout << endl;
    
    // Получаем все перестановки
    vector<vector<char>> perms = getAllPerms(tree1);
    cout << "\n  Все перестановки (getAllPerms):" << endl;
    for (size_t i = 0; i < perms.size(); ++i) {
        cout << "    " << (i+1) << ": ";
        for (char c : perms[i]) cout << c;
        cout << endl;
    }
    
    // Проверяем getPerm1
    cout << "\n  getPerm1:" << endl;
    for (int i = 1; i <= 3; ++i) {
        vector<char> perm = getPerm1(tree1, i);
        cout << "    Перестановка №" << i << ": ";
        for (char c : perm) cout << c;
        cout << endl;
    }
    
    // Проверяем getPerm2
    cout << "\n  getPerm2:" << endl;
    for (int i = 1; i <= 3; ++i) {
        vector<char> perm = getPerm2(tree1, i);
        cout << "    Перестановка №" << i << ": ";
        for (char c : perm) cout << c;
        cout << endl;
    }
    
    // Пример 2: для {'1','3','5','7'}
    vector<char> in2 = {'1', '3', '5', '7'};
    PMTree tree2(in2);
    
    cout << "\n2. Для набора символов: ";
    for (char c : in2) cout << c << " ";
    cout << endl;
    
    // Показываем первые 6 перестановок
    vector<vector<char>> perms2 = getAllPerms(tree2);
    cout << "\n  Первые 6 перестановок (getAllPerms):" << endl;
    for (int i = 0; i < min(6, (int)perms2.size()); ++i) {
        cout << "    " << (i+1) << ": ";
        for (char c : perms2[i]) cout << c;
        cout << endl;
    }
    
    // Проверяем getPerm2 для 1-й и 2-й перестановок
    cout << "\n  getPerm2:" << endl;
    vector<char> perm1 = getPerm2(tree2, 1);
    cout << "    Перестановка №1: ";
    for (char c : perm1) cout << c;
    cout << endl;
    
    vector<char> perm2 = getPerm2(tree2, 2);
    cout << "    Перестановка №2: ";
    for (char c : perm2) cout << c;
    cout << endl;
    
    // Тест для некорректного номера
    cout << "\n  Тест для некорректного номера (100):" << endl;
    vector<char> invalid = getPerm2(tree2, 100);
    if (invalid.empty()) {
        cout << "    Возвращен пустой вектор (перестановка не существует)" << endl;
    }
}

int main() {
    // Установка локали для корректного вывода
    setlocale(LC_ALL, "Russian");
    
    cout << "========================================" << endl;
    cout << "   ДЕРЕВО ВАРИАНТОВ ДЛЯ ПЕРЕСТАНОВОК   " << endl;
    cout << "========================================" << endl;
    
    // Демонстрация работы функций
    demonstrateFunctions();
    
    // Проведение вычислительного эксперимента
    runExperiment();
    
    cout << "\n========================================" << endl;
    cout << "   ЗАВЕРШЕНИЕ РАБОТЫ ПРОГРАММЫ   " << endl;
    cout << "========================================" << endl;
    
    return 0;
}
