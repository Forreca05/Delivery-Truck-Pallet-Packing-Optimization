/**
 * @file parser.cpp
 * @brief Functions to parse CSV input files containing pallet and truck information.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"

/**
 * @brief Parses the pallets data from a CSV file.
 *
 * Each line in the CSV file is expected to have the format:
 * PalletID,Weight,Profit
 *
 * @param filePath The path to the CSV file containing pallet data.
 * @return A vector of Pallet objects parsed from the file.
 */
std::vector<Pallet> parsePalletsCSV(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file.\n";
        return {};
    }

    std::vector<Pallet> pallets;
    std::string line;

    // Skip header
    if (!std::getline(file, line)) {
        return pallets;
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        Pallet p;

        // Skip Pallet ID
        if (!std::getline(ss, token, ',')) continue;

        // Weight
        if (!std::getline(ss, token, ',')) continue;
        p.weight = std::stoi(token);

        // Profit
        if (!std::getline(ss, token, ',')) continue;
        p.profit = std::stoi(token);

        pallets.push_back(p);
    }

    return pallets;
}

/**
 * @brief Parses the truck's capacity from a CSV file.
 *
 * The file is expected to have two lines: a header and one line with:
 * Capacity, Pallets
 *
 * Only the first value (capacity) is extracted.
 *
 * @param filePath The path to the CSV file containing truck information.
 * @return The truck's maximum weight capacity. Returns -1 if the file cannot be read.
 */
int parseTruckAndPalletsCSV(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file.\n";
        return -1;
    }

    std::string line;

    // Skip header
    if (!std::getline(file, line)) {
        return -1;
    }

    // Read actual data line
    std::getline(file, line);
    std::istringstream ss(line);
    
    int capacity = -1;
    ss >> capacity;
    return capacity;
}
