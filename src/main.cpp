/**
 * @file main.cpp
 * @brief Entry point for the Delivery Truck Pallet Packing Optimization project with enhanced CLI and tabular results.
 */

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "algorithms.h"
#include "parser.h"
#include "benchmark.h"

namespace fs = std::filesystem;

// Helper to get a line of input and trim whitespace
static std::string promptLine(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    if (input.empty()) {
        std::getline(std::cin, input);
    }
    return input;
}

// Ask user to pick from minOption..maxOption, returns chosen int
static int promptNumber(const std::string &prompt, int minOption, int maxOption) {
    int choice;
    while (true) {
        std::cout << prompt;
        if (std::cin >> choice && choice >= minOption && choice <= maxOption) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cout << "Invalid selection. Enter a number between "
                  << minOption << " and " << maxOption << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Validate that file exists and is readable
static bool validateFile(const std::string &path) {
    if (!fs::exists(path)) {
        std::cout << "Error: File not found: " << path << "\n";
        return false;
    }
    if (!fs::is_regular_file(path)) {
        std::cout << "Error: Not a regular file: " << path << "\n";
        return false;
    }
    return true;
}

int main() {
    std::cout << "=== Delivery Truck Pallet Packing Optimization Tool ===\n";
    std::cout << "Solve the 0/1 Knapsack problem using various algorithms.\n";
    
    bool running = true;
    while (running) {
        std::cout << "\nChoose input source:\n"
                  << " [1] Predefined dataset (1-10)\n"
                  << " [2] Custom file paths\n"
                  << " [Q] Quit\n";
        std::string selection;
        std::getline(std::cin, selection);
        if (selection == "Q" || selection == "q") {
            std::cout << "Goodbye!\n";
            break;
        }

        std::string palletsPath, truckPath;
        if (selection == "1") {
            int idx = promptNumber("Select dataset number (1-10): ", 1, 10);
            char buf[16];
            std::snprintf(buf, sizeof(buf), "%02d", idx);
            palletsPath = "../data/Pallets_" + std::string(buf) + ".csv";
            truckPath   = "../data/TruckAndPallets_" + std::string(buf) + ".csv";
            std::cout << "Loading dataset " << idx << "...\n";
        }
        else if (selection == "2") {
            do {
                palletsPath = promptLine("Enter path to Pallets CSV: ");
            } while (!validateFile(palletsPath));
            do {
                truckPath = promptLine("Enter path to Truck specification CSV: ");
            } while (!validateFile(truckPath));
        }
        else if (selection == "b") {
            runBenchmarks();
            std::cout << "Benchmark data saved to benchmark.csv\n";
            return 0;
        }
        else {
            std::cout << "Invalid option. Please select 1, 2, or Q.\n";
            continue;
        }

        // parse inputs
        std::vector<Pallet> pallets;
        int capacity = 0;
        try {
            pallets = parsePalletsCSV(palletsPath);
            capacity = parseTruckAndPalletsCSV(truckPath);
        } catch (const std::exception &e) {
            std::cout << "Error parsing files: " << e.what() << "\n";
            continue;
        }

        // choose algorithm
        std::cout << "\nSelect algorithm:\n"
                  << " [1] Exhaustive Search\n"
                  << " [2] Backtracking\n"
                  << " [3] Dynamic Programming\n"
                  << " [4] Approximation\n"
                  << " [5] Integer Linear Programming\n";
        int algo = promptNumber("Enter choice (1-5): ", 1, 5);

        // run and time
        auto start = std::chrono::steady_clock::now();
        std::vector<Pallet> result;
        switch (algo) {
            case 1: result = exhaustiveSearch(pallets, capacity); break;
            case 2: result = backtracking(pallets, capacity);   break;
            case 3: result = dynamicProgramming(pallets, capacity); break;
            case 4: result = approximationAlgorithm(pallets, capacity); break;
            case 5: result = integerLinearProgramming(pallets, capacity); break;
        }
        auto end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        // display results in table
        std::cout << "\n" << (algo == 4 ? "Approximate" : "Optimal")
                  << " solution:\n";
        std::cout << std::left
                  << std::setw(12) << "Pallet ID"
                  << std::setw(12) << "Weight"
                  << std::setw(12) << "Profit" << "\n";
        std::cout << std::string(36, '-') << "\n";

        int count = 0, totalW = 0, totalP = 0;
        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i].weight > 0) {
                ++count;
                totalW += result[i].weight;
                totalP += result[i].profit;
                std::cout << std::left
                          << std::setw(12) << (i + 1)
                          << std::setw(12) << result[i].weight
                          << std::setw(12) << result[i].profit << "\n";
            }
        }
        std::cout << std::string(36, '-') << "\n";
        std::cout << std::left
                  << std::setw(12) << "Total"
                  << std::setw(12) << totalW
                  << std::setw(12) << totalP << "\n";
        std::cout << std::fixed << std::setprecision(6)
                  << "Elapsed time: " << elapsed << "s\n";

        // ask to continue
        std::cout << "\nPress Enter to return to main menu...";
        std::cin.get();
    }
    return 0;
}
