// Copyright 2022 NNTU-CS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include "tree.h"

// Функция для измерения времени выполнения
template<typename Func>
int64_t measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
        end - start).count();
}

// Функция для генерации случайного номера перестановки
int getRandomPermNumber(int maxNum, std::mt19937& gen) {
    std::uniform_int_distribution<> dis(1, maxNum);
    return dis(gen);
}

// Функция для проведения эксперимента
void runExperiment() {
    std::cout << "\n=== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ===" << std::endl;
    std::cout << "Измерение времени выполнения функций в зависимости от n"
              << std::endl << std::endl;

    std::vector<int> sizes = {3, 4, 5, 6, 7, 8};

    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << std::setw(5) << "n"
              << std::setw(20) << "getAllPerms (мкс)"
              << std::setw(20) << "getPerm1 (мкс)"
              << std::setw(20) << "getPerm2 (мкс)" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (int n : sizes) {
        // Создаем вектор символов
        std::vector<char> elements;
        for (int i = 0; i < n; ++i) {
            elements.push_back('0' + (i % 10));
        }

        // Строим дерево
        PMTree tree(elements);

        // Генерируем случайный номер перестановки
        int totalPerms = factorial(n);
        int randomNum = getRandomPermNumber(totalPerms, gen);

        // Измеряем время getAllPerms
        int64_t timeAllPerms = measureTime([&]() {
            getAllPerms(tree);
        });

        // Измеряем время getPerm1
        int64_t timePerm1 = measureTime([&]() {
            getPerm1(tree, randomNum);
        });

        // Измеряем время getPerm2
        int64_t timePerm2 = measureTime([&]() {
            getPerm2(tree, randomNum);
        });

        // Выводим результаты
        std::cout << std::setw(5) << n
                  << std::setw(20) << timeAllPerms
                  << std::setw(20) << timePerm1
                  << std::setw(20) << timePerm2 << std::endl;
    }

    std::cout << "\nПримечание: getPerm1 использует getAllPerms, "
              << "поэтому их время выполнения близко." << std::endl;
    std::cout << "getPerm2 работает значительно быстрее, "
              << "так как использует прямую навигацию по дереву."
              << std::endl;
}

// Функция демонстрации работы функций
void demonstrateFunctions() {
    std::cout << "\n=== ДЕМОНСТРАЦИЯ РАБОТЫ ФУНКЦИЙ ===" << std::endl;

    // Пример 1: перестановки для {'1','2','3'}
    std::vector<char> in1 = {'1', '2', '3'};
    PMTree tree1(in1);

    std::cout << "\n1. Для набора символов: ";
    for (char c : in1) std::cout << c << " ";
    std::cout << std::endl;

    // Получаем все перестановки
    std::vector<std::vector<char>> perms = getAllPerms(tree1);
    std::cout << "\n  Все перестановки (getAllPerms):" << std::endl;
    for (size_t i = 0; i < perms.size(); ++i) {
        std::cout << "    " << (i + 1) << ": ";
        for (char c : perms[i]) std::cout << c;
        std::cout << std::endl;
    }

    // Проверяем getPerm1
    std::cout << "\n  getPerm1:" << std::endl;
    for (int i = 1; i <= 3; ++i) {
        std::vector<char> perm = getPerm1(tree1, i);
        std::cout << "    Перестановка №" << i << ": ";
        for (char c : perm) std::cout << c;
        std::cout << std::endl;
    }

    // Проверяем getPerm2
    std::cout << "\n  getPerm2:" << std::endl;
    for (int i = 1; i <= 3; ++i) {
        std::vector<char> perm = getPerm2(tree1, i);
        std::cout << "    Перестановка №" << i << ": ";
        for (char c : perm) std::cout << c;
        std::cout << std::endl;
    }

    // Пример 2: для {'1','3','5','7'}
    std::vector<char> in2 = {'1', '3', '5', '7'};
    PMTree tree2(in2);

    std::cout << "\n2. Для набора символов: ";
    for (char c : in2) std::cout << c << " ";
    std::cout << std::endl;

    // Показываем первые 6 перестановок
    std::vector<std::vector<char>> perms2 = getAllPerms(tree2);
    std::cout << "\n  Первые 6 перестановок (getAllPerms):" << std::endl;
    int maxShow = std::min(6, static_cast<int>(perms2.size()));
    for (int i = 0; i < maxShow; ++i) {
        std::cout << "    " << (i + 1) << ": ";
        for (char c : perms2[i]) std::cout << c;
        std::cout << std::endl;
    }

    // Проверяем getPerm2 для 1-й и 2-й перестановок
    std::cout << "\n  getPerm2:" << std::endl;
    std::vector<char> perm1 = getPerm2(tree2, 1);
    std::cout << "    Перестановка №1: ";
    for (char c : perm1) std::cout << c;
    std::cout << std::endl;

    std::vector<char> perm2 = getPerm2(tree2, 2);
    std::cout << "    Перестановка №2: ";
    for (char c : perm2) std::cout << c;
    std::cout << std::endl;

    // Тест для некорректного номера
    std::cout << "\n  Тест для некорректного номера (100):" << std::endl;
    std::vector<char> invalid = getPerm2(tree2, 100);
    if (invalid.empty()) {
        std::cout << "    Возвращен пустой вектор "
                  << "(перестановка не существует)" << std::endl;
    }
}

int main() {
    // Установка локали для корректного вывода
    std::setlocale(LC_ALL, "Russian");

    std::cout << "========================================" << std::endl;
    std::cout << "   ДЕРЕВО ВАРИАНТОВ ДЛЯ ПЕРЕСТАНОВОК   " << std::endl;
    std::cout << "========================================" << std::endl;

    // Демонстрация работы функций
    demonstrateFunctions();

    // Проведение вычислительного эксперимента
    runExperiment();

    std::cout << "\n========================================" << std::endl;
    std::cout << "   ЗАВЕРШЕНИЕ РАБОТЫ ПРОГРАММЫ   " << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
