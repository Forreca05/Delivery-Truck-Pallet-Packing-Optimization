#include "algorithms.h"
#include <algorithm>
#include <iostream>
#include <functional>

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

// ====================================================================== //
// ======================= APPROXIMATION ALGORITHM ====================== //
// ====================================================================== //

std::vector<Pallet> greedySolutionA(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();

    std::vector<std::pair<Pallet, int>> items;
    for (int i = 0; i < n; i++) {
        items.push_back({pallets[i], i});
    }

    std::sort(items.begin(), items.end(), [](std::pair<Pallet, int> a, std::pair<Pallet, int> b){
        return (double)a.first.profit / a.first.weight > (double)b.first.profit / b.first.weight;
    });

    int currentWeight = 0;
    std::vector<Pallet> result(pallets.size(), {0, 0});

    for (int i = 0; i < n; i++) {
        if (currentWeight + items[i].first.weight > capacity) break;
        currentWeight += items[i].first.weight;
        result[items[i].second] = items[i].first;
    }
    
    return result;
}

std::vector<Pallet> greedySolutionB(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();

    std::vector<std::pair<Pallet, int>> items;
    for (int i = 0; i < n; i++) {
        items.push_back({pallets[i], i});
    }

    std::sort(items.begin(), items.end(), [](std::pair<Pallet, int> a, std::pair<Pallet, int> b){
        if (a.first.profit == b.first.profit) {
            return a.first.weight < b.first.weight;
        }
        return a.first.profit > b.first.profit;
    });

    int currentWeight = 0;
    std::vector<Pallet> result(pallets.size(), {0, 0});

    for (int i = 0; i < n; i++) {
        if (currentWeight + items[i].first.weight > capacity) break;
        currentWeight += items[i].first.weight;
        result[items[i].second] = items[i].first;
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

// ====================================================================== //
// ===================== INTEGER LINEAR PROGRAMMING ===================== //
// ====================================================================== //

// This function returns an upper bound for the remaining knapsack problem, by solving the fractional knapsack since it will always be at worst the same as the optimal solution
double lpBound(const std::vector<std::pair<Pallet, int>> sortedPallets, int startIndex, long long currentWeight, int capacity) {
    // Assume the vector is sorted according to v/w
    int n = sortedPallets.size();
    double remainingCapacity = capacity - currentWeight;
    double bound = 0;
    for (int i = startIndex; i < n; i++) {
        int weight = sortedPallets[i].first.weight;
        int profit = sortedPallets[i].first.profit;
        if (weight > remainingCapacity) {
            bound += profit * (remainingCapacity / capacity);
            break;
        }
        else {
            remainingCapacity -= weight;
            bound += profit;
        }
    }
    return bound;
}

void branchAndBoundSearch(
    const std::vector<std::pair<Pallet, int>>& sortedPallets,
    int currentIndex,
    long long currentWeight,
    long long currentProfit,
    int capacity,
    std::vector<int>& currentTake,
    std::vector<int>& bestTake,
    long long& bestProfit
) {
    int n = sortedPallets.size();
    if (currentIndex == n) {
        if (currentProfit > bestProfit) {
            bestProfit = currentProfit;
            bestTake = currentTake;
        }
        return;
    }
    double estimate = currentProfit + lpBound(sortedPallets, currentIndex, currentWeight, capacity);
    if (estimate <= bestProfit) return;

    Pallet pallet = sortedPallets[currentIndex].first;
    if (currentWeight + sortedPallets[currentIndex].first.weight <= capacity) {
        currentTake[currentIndex] = 1;
        branchAndBoundSearch(
            sortedPallets,
            currentIndex + 1,
            currentWeight + pallet.weight,
            currentProfit + pallet.profit,
            capacity,
            currentTake,
            bestTake,
            bestProfit
        );
        currentTake[currentIndex] = 0;
    }
    branchAndBoundSearch(
        sortedPallets,
        currentIndex + 1,
        currentWeight,
        currentProfit,
        capacity,
        currentTake,
        bestTake,
        bestProfit
    );
}

std::vector<Pallet> integerLinearProgramming(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();

    std::vector<std::pair<Pallet,int>> items;
    for (int i = 0; i < n; ++i) {
        items.push_back({pallets[i], i});
    }
    
    std::sort(items.begin(), items.end(), [](std::pair<Pallet, int> a, std::pair<Pallet, int> b){
        return (double)a.first.profit / a.first.weight > (double)b.first.profit / b.first.weight;
    });

    std::vector<int> bestTake(n, 0), currTake(n, 0);
    long long bestProfit = 0;

    branchAndBoundSearch(items, 0, 0, 0, capacity, currTake, bestTake, bestProfit);

    std::vector<Pallet> result(n);
    for (int i = 0; i < n; i++) {
        if (bestTake[i]) {
            int originalIndex = items[i].second;
            result[originalIndex] = items[i].first;
        }
    }
    return result;
}