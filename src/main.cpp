#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "algorithms.h"
#include "parser.h"

int main() {
    std::cout << "Please choose which dataset to use (1 - 10)" << std::endl;
    std::string dataIndex, filePathPallets, filePathTruckAndPallets;
    std::cin >> dataIndex;
    std::cout << std::endl;

    if (stoi(dataIndex) < 1 || stoi(dataIndex) > 10) {
        std::cout << "Please select a dataset between 1 and 10" << std::endl;
        return 1;
    }
    else if (dataIndex == "10") {
        filePathPallets = "../data/Pallets_10.csv";
        filePathTruckAndPallets = "../data/TruckAndPallets_10.csv";
    }
    else {
        filePathPallets = "../data/Pallets_0" + dataIndex + ".csv";
        filePathTruckAndPallets = "../data/TruckAndPallets_0" + dataIndex + ".csv";
    }
    
    std::vector<Pallet> pallets = parsePalletsCSV(filePathPallets);
    int capacity = parseTruckAndPalletsCSV(filePathTruckAndPallets);

    std::cout << "Please choose which algorithm to use" << std::endl;
    std::cout << "[1] Exhaustive Search" << std::endl;
    std::cout << "[2] Dynamic Programming" << std::endl;
    std::cout << "[3] Approximation Algorithm" << std::endl;
    std::cout << "[4] Integer Linear Programming" << std::endl;
    int algorithmIndex;
    std::cin >> algorithmIndex;

    std::vector<Pallet> result;
    const auto start = std::chrono::system_clock::now();
    switch (algorithmIndex) {
        case 1:
            result = exhaustiveSearch(pallets, capacity);
            break;
        case 2:
            result = dynamicProgramming(pallets, capacity);
            break;
        case 3:
            result = approximationAlgorithm(pallets, capacity);
            break;
        case 4:
            result = integerLinearProgramming(pallets, capacity);
            break;
        default:
            std::cout << "Please select one of the presented algorithms" << std::endl;
            return 1;
    }
    std::cout << std::endl;

    const auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << std::fixed << std::setprecision(6) << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
    std::cout << std::endl;

    std::cout << "Optimal solution:" << std::endl;

    int nPallets = pallets.size();
    int totalWeight = 0;
    int totalProfit = 0;
    for (int i = 0; i < nPallets; i++) {
        if (result[i].weight == 0 && result[i].profit == 0) continue;
        totalWeight += result[i].weight;
        totalProfit += result[i].profit;
        std::cout << i + 1 << ' ' << result[i].weight << ' ' << result[i].profit << std::endl;
    }
    std::cout << "Total weight = " << totalWeight << std::endl;
    std::cout << "Total profit = " << totalProfit << std::endl;
    return 0;
}
