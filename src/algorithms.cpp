#include "algorithms.h"

std::vector<Pallet> exhaustiveSearch(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    int bestProfit = 0;
    int bestMask = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        int totalWeight = 0;
        int totalProfit = 0;
        
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) {
                totalWeight += pallets[i].weight;
                totalProfit += pallets[i].profit;
            }
        }

        if (totalWeight < capacity && totalProfit > bestProfit) {
            bestProfit = totalProfit;
            bestMask = mask;
        }
    }
    
    std::vector<Pallet> result(n);
    for (int i = 0; i < n; i++) {
        if ((bestMask >> i) & 1) {
            result[i] = pallets[i];
        }
        else {
            result[i] = {0, 0};
        }
    }
    return result;
}