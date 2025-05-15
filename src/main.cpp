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

    if (dataIndex < "1" || dataIndex > "10") {
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
    int algorithmIndex;
    std::cin >> algorithmIndex;

    std::vector<Pallet> result;
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
        default:
            std::cout << "Please select one of the presented algorithms" << std::endl;
            return 1;
    }
    std::cout << std::endl;

    std::cout << "Optimal solution:" << std::endl;

    int nPallets = pallets.size();
    for (int i = 0; i < nPallets; i++) {
        if (result[i].weight == 0 && result[i].profit == 0) continue;
        std::cout << i + 1 << ' ' << result[i].weight << ' ' << result[i].profit << std::endl;
    }
    return 0;
}
