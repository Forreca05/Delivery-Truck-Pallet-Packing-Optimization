/**
 * @file algorithms.cpp
 * @brief Implements several strategies to solve the 0/1 Knapsack problem for pallet selection.
 */

#include <algorithm>
#include "algorithms.h"
#include <climits>

// ====================================================================== //
// ========================= EXHAUSTIVE SEARCH ========================== //
// ====================================================================== //

/**
 * @brief Solves the knapsack problem using exhaustive (brute-force) search.
 * 
 * Iterates through all 2^n combinations of pallets, checking which subset
 * fits within the weight capacity and yields the maximum profit. Among
 * multiple optimal solutions, selects the one with lowest total weight.
 * 
 * @param pallets Vector of pallet objects with weight and profit
 * @param capacity Maximum weight capacity of the truck
 * @return Vector of selected pallets (non-selected are returned as {0,0})
 * 
 * @complexity Time: O(2^n * n) - Evaluates all subsets
 * @complexity Space: O(n) - Stores the best combination mask
 */
std::vector<Pallet> exhaustiveSearch(const std::vector<Pallet>& pallets, int capacity) {
    size_t n = pallets.size();
    int bestProfit = 0;
    int bestWeight = INT_MAX;
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

        if (totalWeight <= capacity) {
            if (totalProfit > bestProfit || (totalProfit == bestProfit && totalWeight < bestWeight)) {
                bestProfit = totalProfit;
                bestWeight = totalWeight;
                bestMask = mask;
            }
        }
    }
    
    std::vector<Pallet> result(n);
    for (size_t i = 0; i < n; i++) {
        result[i] = ((bestMask >> i) & 1) ? pallets[i] : Pallet{0, 0};
    }
    return result;
}

// ====================================================================== //
// ============================ BACKTRACKING ============================ //
// ====================================================================== //

/**
 * @brief Recursive helper for backtracking solution of 0/1 knapsack.
 * 
 * Explores all subsets through recursive inclusion/exclusion of items,
 * tracking both maximum profit and minimum weight for optimal solutions.
 * 
 * @param pallets Vector of pallet objects
 * @param currentIndex Current recursion depth
 * @param currentWeight Accumulated weight in current path
 * @param currentProfit Accumulated profit in current path
 * @param capacity Maximum allowed weight
 * @param currentTake Current selection status of items
 * @param bestTake Best found selection status
 * @param bestProfit Reference to best profit found
 * @param bestWeight Reference to best weight for optimal solutions
 * 
 * @complexity Time: O(2^n) - Worst case explores all combinations
 * @complexity Space: O(n) - Recursion depth and tracking vectors
 */
void backtrackingHelper(
    const std::vector<Pallet>& pallets,
    int currentIndex,
    long long currentWeight,
    long long currentProfit,
    int capacity,
    std::vector<int>& currentTake,
    std::vector<int>& bestTake,
    long long& bestProfit,
    long long& bestWeight
) {
    int n = pallets.size();
    if (currentIndex == n) {
        if (currentProfit > bestProfit || (currentProfit == bestProfit && currentWeight < bestWeight)) {
            bestProfit = currentProfit;
            bestWeight = currentWeight;
            bestTake = currentTake;
        }
        return;
    }

    Pallet pallet = pallets[currentIndex];
    if (currentWeight + pallet.weight <= capacity) {
        currentTake[currentIndex] = 1;
        backtrackingHelper(
            pallets, currentIndex + 1,
            currentWeight + pallet.weight,
            currentProfit + pallet.profit,
            capacity, currentTake, bestTake,
            bestProfit, bestWeight
        );
        currentTake[currentIndex] = 0;
    }

    backtrackingHelper(
        pallets, currentIndex + 1,
        currentWeight, currentProfit,
        capacity, currentTake, bestTake,
        bestProfit, bestWeight
    );
}

std::vector<Pallet> backtracking(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<int> bestTake(n, 0), currTake(n, 0);
    long long bestProfit = 0;
    long long bestWeight = LLONG_MAX;

    backtrackingHelper(pallets, 0, 0, 0, capacity, currTake, bestTake, bestProfit, bestWeight);

    std::vector<Pallet> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = bestTake[i] ? pallets[i] : Pallet{0, 0};
    }
    return result;
}

// ====================================================================== //
// ========================= DYNAMIC PROGRAMMING ======================== //
// ====================================================================== //

/**
 * @brief Solves 0/1 knapsack via backtracking with optimal weight selection.
 * 
 * Initializes tracking structures and invokes recursive exploration of
 * all possible pallet combinations. Returns the optimal subset with
 * maximum profit and minimum weight when multiple optima exist.
 * 
 * @param pallets Vector of pallet objects
 * @param capacity Truck weight capacity
 * @return Selected pallets vector with optimal profit/weight
 * 
 * @complexity Time: O(2^n) - Pruned search space through backtracking
 * @complexity Space: O(n) - Recursion stack and tracking vectors
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

    int maxProfit = dp[n][capacity];
    int minWeight = capacity + 1;
    for (int j = 0; j <= capacity; j++) {
        if (dp[n][j] == maxProfit && j < minWeight) {
            minWeight = j;
        }
    }

    std::vector<Pallet> result(n);
    int i = n, j = minWeight;
    while (i > 0 && j >= 0) {
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
 * @brief Greedy heuristic using profit/weight ratio sorting.
 * 
 * Sorts pallets by descending profit/weight ratio, selects items until
 * capacity is full.
 * 
 * @param pallets Vector of pallet objects
 * @param capacity Truck weight limit
 * @return Subset of pallets selected by greedy criteria
 * 
 * @complexity Time: O(n log n) - Sorting dominates complexity
 * @complexity Space: O(n) - Stores sorted items and result
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
 * @brief Greedy heuristic using profit-first sorting.
 * 
 * Sorts pallets by descending profit (with weight tie-breaker),
 * selects items until capacity is full.
 * 
 * @param pallets Vector of pallet objects
 * @param capacity Truck weight limit
 * @return Subset of pallets selected by profit-first criteria
 * 
 * @complexity Time: O(n log n) - Sorting dominates complexity
 * @complexity Space: O(n) - Stores sorted items and result
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
 * @brief Returns best of two greedy approximation approaches.
 * 
 * Combines results from ratio-based and profit-based greedy strategies,
 * returning the solution with higher total profit. Guarantees a 2-approximation.
 * 
 * @param pallets Vector of pallet objects
 * @param capacity Truck weight limit
 * @return Better of two greedy solutions (A or B)
 * 
 * @complexity Time: O(n log n) - Two sorts and linear scans
 * @complexity Space: O(n) - Stores two solution vectors
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
 * @brief Computes upper bound via fractional knapsack relaxation.
 * 
 * Used in branch-and-bound to estimate maximum possible profit for
 * current partial solution through LP relaxation.
 * 
 * @param sortedPallets Items sorted by profit/weight ratio
 * @param startIndex Starting index for remaining items
 * @param currentWeight Weight accumulated in current path
 * @param capacity Total truck capacity
 * @return Upper bound profit estimate (fractional items allowed)
 * 
 * @complexity Time: O(n) - Linear scan of remaining items
 * @complexity Space: O(1) - No additional storage
 */
double lpBound(const std::vector<std::pair<Pallet, int>>& sortedPallets, int startIndex, long long currentWeight, int capacity) {
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
 * @brief Branch-and-bound search with profit/weight optimization.
 * 
 * Recursively explores item inclusion/exclusion while:
 * 1. Maintaining best known valid solution
 * 2. Pruning branches that cannot exceed current best profit
 * 3. Prioritizing items with higher profit/weight ratio
 * 
 * @param sortedPallets Items pre-sorted by profit/weight ratio
 * @param currentIndex Current position in item list
 * @param currentWeight Accumulated weight in current path
 * @param currentProfit Accumulated profit in current path
 * @param capacity Truck weight limit
 * @param currentTake Current selection status
 * @param bestTake Best found selection status
 * @param bestProfit Reference to best profit found
 * @param bestWeight Reference to best weight for optimal solutions
 * 
 * @complexity Time: O(2^n) - Exponential with pruning effectiveness
 * @complexity Space: O(n) - Recursion depth and tracking vectors
 */
void branchAndBoundSearch(
    const std::vector<std::pair<Pallet, int>>& sortedPallets,
    int currentIndex,
    long long currentWeight,
    long long currentProfit,
    int capacity,
    std::vector<int>& currentTake,
    std::vector<int>& bestTake,
    long long& bestProfit,
    long long& bestWeight
) {
    int n = sortedPallets.size();
    if (currentIndex == n) {
        if (currentProfit > bestProfit || (currentProfit == bestProfit && currentWeight < bestWeight)) {
            bestProfit = currentProfit;
            bestWeight = currentWeight;
            bestTake = currentTake;
        }
        return;
    }

    double estimate = currentProfit + lpBound(sortedPallets, currentIndex, currentWeight, capacity);
    if (estimate <= bestProfit) return;

    Pallet pallet = sortedPallets[currentIndex].first;
    if (currentWeight + pallet.weight <= capacity) {
        currentTake[currentIndex] = 1;
        branchAndBoundSearch(
            sortedPallets, currentIndex + 1,
            currentWeight + pallet.weight,
            currentProfit + pallet.profit,
            capacity, currentTake, bestTake,
            bestProfit, bestWeight
        );
        currentTake[currentIndex] = 0;
    }

    branchAndBoundSearch(
        sortedPallets, currentIndex + 1,
        currentWeight, currentProfit,
        capacity, currentTake, bestTake,
        bestProfit, bestWeight
    );
}

/**
 * @brief Solves 0/1 knapsack via branch-and-bound (ILP approach).
 * 
 * Uses profit/weight sorted items with LP relaxation for upper bounds,
 * initialized with greedy solution for effective pruning. Returns
 * optimal subset with maximum profit and minimal weight when multiple
 * optima exist.
 * 
 * @param pallets Vector of pallet objects
 * @param capacity Truck weight capacity
 * @return Optimal pallet selection with profit/weight optimization
 * 
 * @complexity Time: O(2^n) - Worst case exponential, pruning reduces
 * @complexity Space: O(n) - Sorting and tracking structures
 */
std::vector<Pallet> integerLinearProgramming(const std::vector<Pallet>& pallets, int capacity) {
    int n = pallets.size();
    std::vector<std::pair<Pallet, int>> items;
    for (int i = 0; i < n; ++i) {
        items.push_back({pallets[i], i});
    }

    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return (double)a.first.profit / a.first.weight > (double)b.first.profit / b.first.weight;
    });

    std::vector<int> currTake(n, 0), bestTake(n, 0);
    long long bestProfit = 0;
    long long bestWeight = LLONG_MAX;

    // Greedy initialization for better pruning
    long long currentWeight = 0;
    for (int i = 0; i < n; i++) {
        const auto& pallet = items[i].first;
        if (currentWeight + pallet.weight <= capacity) {
            bestTake[i] = 1;
            currentWeight += pallet.weight;
            bestProfit += pallet.profit;
        }
    }
    bestWeight = currentWeight;

    branchAndBoundSearch(items, 0, 0, 0, capacity, currTake, bestTake, bestProfit, bestWeight);

    std::vector<Pallet> result(n);
    for (int i = 0; i < n; ++i) {
        if (bestTake[i]) {
            result[items[i].second] = items[i].first;
        }
    }
    return result;
}