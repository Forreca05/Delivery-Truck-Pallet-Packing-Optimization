#include "algorithms.h"
#include <iostream>

std::vector<Pallet> exhaustiveSearch(const std::vector<Pallet>& pallets, int capacity) {
    size_t n = pallets.size();
    int bestProfit = 0;
    int bestMask = 0;

    for (size_t mask = 0; mask < ((size_t)1 << n); mask++) {
        int totalWeight = 0;
        int totalProfit = 0;
        
        for (size_t i = 0; i < n; i++) {
            if ((mask >> i) & 1) {
                totalWeight += pallets[i].weight;
                totalProfit += pallets[i].profit;
            }
        }

        if (totalWeight <= capacity && totalProfit > bestProfit) {
            bestProfit = totalProfit;
            bestMask = mask;
        }
    }
    
    std::vector<Pallet> result(n);
    for (size_t i = 0; i < n; i++) {
        if ((bestMask >> i) & 1) {
            result[i] = pallets[i];
        }
        else {
            result[i] = {0, 0};
        }
    }
    return result;
}

std::vector<Pallet> dynamicProgramming(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    int dp[n + 1][capacity + 1] = {};

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j - pallets[i - 1].weight < 0) continue;
            dp[i][j] = std::max(dp[i][j], dp[i - 1][j - pallets[i - 1].weight] + pallets[i - 1].profit);
        }
    }

    std::vector<Pallet> result(n);
    int i = n;
    int j = capacity;

    while (i > 0 && j > 0) {
        if (dp[i][j] != dp[i - 1][j]) {
            result[i - 1] = pallets[i - 1];
            j -= pallets[i - 1].weight;
        }
        i--;
    }
    return result;
}