#include <iostream>
#include <string>
#include <vector>
#include "algorithms.h"
#include "parser.h"

int main() {
    std::string filePathPallets = "../data/Pallets_01.csv";
    std::string filePathTruckAndPallets = "../data/TruckAndPallets_01.csv";
    
    std::vector<Pallet> pallets = parsePalletsCSV(filePathPallets);
    int capacity = parseTruckAndPalletsCSV(filePathTruckAndPallets);

    std::vector<Pallet> result = exhaustiveSearch(pallets, capacity);

    std::cout << "Optimal solution:" << std::endl;

    int nPallets = pallets.size();
    for (int i = 0; i < nPallets; i++) {
        if (result[i].weight == 0 && result[i].profit == 0) continue;
        std::cout << i + 1 << ' ' << result[i].weight << ' ' << result[i].profit << std::endl;
    }
    return 0;
}
