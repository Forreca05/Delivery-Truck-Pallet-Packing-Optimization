/**
 * @file pallet.h
 * @brief Definition of the Pallet data structure.
 */

#ifndef PALLET_H
#define PALLET_H

/**
 * @struct Pallet
 * @brief Represents a pallet with weight and profit.
 *
 * This structure is used in the 0/1 knapsack problem context to represent
 * a pallet that can be loaded onto a truck. Each pallet has a specific weight
 * and generates a certain profit.
 */
struct Pallet {
    int weight;  ///< The weight of the pallet
    int profit;  ///< The profit obtained from the pallet
};

#endif // PALLET_H
