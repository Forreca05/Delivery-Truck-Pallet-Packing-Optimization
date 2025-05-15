#include "algorithms.h"
#include <algorithm>
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

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1));

    for (int i = 1; i <= n; i++) {
        Pallet currentPallet = pallets[i - 1];
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j - pallets[i - 1].weight < 0) continue;
            dp[i][j] = std::max(dp[i][j], dp[i - 1][j - currentPallet.weight] + currentPallet.profit);
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

std::vector<Pallet> greedySolutionA(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();

    std::vector<std::pair<Pallet, int>> copy;
    for (int i = 0; i < n; i++) {
        copy.push_back({pallets[i], i});
    }
    std::sort(copy.begin(), copy.end(), [](std::pair<Pallet, int> a, std::pair<Pallet, int> b){
        return (double)a.first.profit / a.first.weight > (double)b.first.profit / b.first.weight;
    });

    int currentWeight = 0;
    std::vector<Pallet> result(pallets.size(), {0, 0});

    for (int i = 0; i < n; i++) {
        if (currentWeight + copy[i].first.weight > capacity) break;
        currentWeight += copy[i].first.weight;
        result[copy[i].second] = copy[i].first;
    }
    
    return result;
}

std::vector<Pallet> greedySolutionB(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();

    std::vector<std::pair<Pallet, int>> copy;
    for (int i = 0; i < n; i++) {
        copy.push_back({pallets[i], i});
    }
    std::sort(copy.begin(), copy.end(), [](std::pair<Pallet, int> a, std::pair<Pallet, int> b){
        if (a.first.profit == b.first.profit) {
            return a.first.weight < b.first.weight;
        }
        return a.first.profit > b.first.profit;
    });

    int currentWeight = 0;
    std::vector<Pallet> result(pallets.size(), {0, 0});

    for (int i = 0; i < n; i++) {
        if (currentWeight + copy[i].first.weight > capacity) break;
        currentWeight += copy[i].first.weight;
        result[copy[i].second] = copy[i].first;
    }
    
    return result;
}

std::vector<Pallet> approximationAlgorithm(const std::vector<Pallet>& pallets, int capacity) {
    std::vector<Pallet> resultA = greedySolutionA(pallets, capacity);
    std::vector<Pallet> resultB = greedySolutionB(pallets, capacity);

    int profitA = 0;
    int profitB = 0;

    for (Pallet p : resultA) profitA += p.profit;
    for (Pallet p : resultB) profitB += p.profit;

    if (profitA > profitB) return resultA;
    else return resultB;
}