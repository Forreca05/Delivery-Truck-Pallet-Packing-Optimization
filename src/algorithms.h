#include <vector>
#include "pallet.h"

std::vector<Pallet> exhaustiveSearch(const std::vector<Pallet>& pallets, int capacity);
std::vector<Pallet> backtracking(const std::vector<Pallet>& pallets, int capacity);
std::vector<Pallet> dynamicProgramming(const std::vector<Pallet>& pallets, int capacity);
std::vector<Pallet> approximationAlgorithm(const std::vector<Pallet>& pallets, int capacity);
