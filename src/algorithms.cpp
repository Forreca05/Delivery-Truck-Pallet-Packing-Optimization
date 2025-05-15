/**
 * @file algorithms.cpp
 * @brief Implements several strategies to solve the 0/1 Knapsack problem for pallet selection.
 */

#include "algorithms.h"
#include <algorithm>
#include <iostream>
#include <functional>

// ====================================================================== //
// ========================= EXHAUSTIVE SEARCH ========================== //
// ====================================================================== //

/**
 * @brief Solves the knapsack problem using exhaustive (brute-force) search.
 * 
 * Iterates through all 2^n combinations of pallets, checking which subset
 * fits within the weight capacity and yields the maximum profit.
 * 
 * @param pallets Vector of pallet objects.
 * @param capacity Maximum weight capacity of the truck.
 * @return Vector of selected pallets (others are returned as {0,0}).
 * 
 * @complexity Time: O(2^n * n), Space: O(n)
 */
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
        } else {
            result[i] = {0, 0};
        }
    }
 
    return result;
}

// ====================================================================== //
// ============================ BACKTRACKING ============================ //
// ====================================================================== //

std::vector<Pallet> backtracking(const std::vector<Pallet>& pallets, int capacity) {
    return {};
}

// ====================================================================== //
// ========================= DYNAMIC PROGRAMMING ======================== //
// ====================================================================== //

/**
 * @brief Solves the knapsack problem using dynamic programming.
 * 
 * Builds a table dp[i][w] representing the maximum profit using the first i pallets
 * with total weight ≤ w.
 * 
 * @param pallets Vector of pallet objects.
 * @param capacity Maximum truck capacity.
 * @return Vector of selected pallets (others are {0,0}).
 * 
 * @complexity Time: O(n * capacity), Space: O(n * capacity)
 */
std::vector<Pallet> dynamicProgramming(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1));

    for (int i = 1; i <= n; i++) {
        Pallet currentPallet = pallets[i - 1];
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j - currentPallet.weight >= 0) {
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - currentPallet.weight] + currentPallet.profit);
            }
        }
    }

    std::vector<Pallet> result(n);
    int i = n, j = capacity;

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

/**
 * @brief Greedy solution A: selects pallets by highest profit/weight ratio.
 * 
 * @param pallets Vector of pallets.
 * @param capacity Maximum truck capacity.
 * @return Vector of selected pallets (others are {0,0}).
 * 
 * @complexity Time: O(n log n), Space: O(n)
 */
std::vector<Pallet> greedySolutionA(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<std::pair<Pallet, int>> items;

    for (int i = 0; i < n; i++) {
        items.push_back({pallets[i], i});
    }

    std::sort(items.begin(), items.end(), [](auto a, auto b){
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

/**
 * @brief Greedy solution B: selects pallets by highest profit, then lowest weight.
 * 
 * @param pallets Vector of pallets.
 * @param capacity Maximum truck capacity.
 * @return Vector of selected pallets (others are {0,0}).
 * 
 * @complexity Time: O(n log n), Space: O(n)
 */
std::vector<Pallet> greedySolutionB(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<std::pair<Pallet, int>> items;

    for (int i = 0; i < n; i++) {
        items.push_back({pallets[i], i});
    }

    std::sort(items.begin(), items.end(), [](auto a, auto b){
        if (a.first.profit == b.first.profit)
            return a.first.weight < b.first.weight;
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

/**
 * @brief Chooses the better result between greedy A and greedy B.
 * 
 * @param pallets Vector of pallets.
 * @param capacity Maximum truck capacity.
 * @return Best of the two greedy solutions.
 * 
 * @complexity Time: O(n log n), Space: O(n)
 */
std::vector<Pallet> approximationAlgorithm(const std::vector<Pallet>& pallets, int capacity) {
    std::vector<Pallet> resultA = greedySolutionA(pallets, capacity);
    std::vector<Pallet> resultB = greedySolutionB(pallets, capacity);

    int profitA = 0, profitB = 0;
    for (const Pallet& p : resultA) profitA += p.profit;
    for (const Pallet& p : resultB) profitB += p.profit;

    return (profitA > profitB) ? resultA : resultB;
}

// ====================================================================== //
// ===================== INTEGER LINEAR PROGRAMMING ===================== //
// ====================================================================== //

/**
 * @brief Computes an upper bound using fractional knapsack (LP-relaxation).
 * 
 * @param sortedPallets Items sorted by profit/weight.
 * @param startIndex Index to begin from.
 * @param currentWeight Current accumulated weight.
 * @param capacity Total capacity of the truck.
 * @return Fractional profit upper bound.
 * 
 * @complexity Time: O(n), Space: O(1)
 */
double lpBound(const std::vector<std::pair<Pallet, int>> sortedPallets, int startIndex, long long currentWeight, int capacity) {
    double remainingCapacity = capacity - currentWeight;
    double bound = 0;

    for (int i = startIndex; i < (int)sortedPallets.size(); i++) {
        int weight = sortedPallets[i].first.weight;
        int profit = sortedPallets[i].first.profit;

        if (weight > remainingCapacity) {
            bound += profit * (remainingCapacity / weight);
            break;
        } else {
            remainingCapacity -= weight;
            bound += profit;
        }
    }

    return bound;
}

/**
 * @brief Recursive Branch and Bound solver for ILP version of knapsack.
 *
 * @param sortedPallets Sorted pallet list with original indices.
 * @param currentIndex Current index in recursion.
 * @param currentWeight Current total weight.
 * @param currentProfit Current total profit.
 * @param capacity Maximum truck capacity.
 * @param currentTake Vector storing current binary decisions.
 * @param bestTake Vector storing best binary decisions found.
 * @param bestProfit Tracks the best profit found.
 * 
 * @complexity Time: Exponential (pruned), Space: O(n)
 */
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
    if (currentWeight + pallet.weight <= capacity) {
        currentTake[currentIndex] = 1;
        branchAndBoundSearch(sortedPallets, currentIndex + 1, currentWeight + pallet.weight,
                             currentProfit + pallet.profit, capacity, currentTake, bestTake, bestProfit);
        currentTake[currentIndex] = 0;
    }

    branchAndBoundSearch(sortedPallets, currentIndex + 1, currentWeight, currentProfit,
                         capacity, currentTake, bestTake, bestProfit);
}

/**
 * @brief Solves the 0/1 knapsack problem using Branch and Bound (ILP).
 * 
 * Sorts items by value/weight ratio, estimates upper bounds using fractional knapsack,
 * and prunes unpromising branches.
 * 
 * @param pallets Vector of pallet items.
 * @param capacity Maximum truck weight.
 * @return Vector of selected pallets.
 * 
 * @complexity Time: Exponential (with pruning), Space: O(n)
 */
std::vector<Pallet> integerLinearProgramming(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<std::pair<Pallet, int>> items;

    for (int i = 0; i < n; ++i)
        items.push_back({pallets[i], i});

    std::sort(items.begin(), items.end(), [](auto a, auto b){
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
