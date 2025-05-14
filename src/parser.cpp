#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"

std::vector<Pallet> parsePalletsCSV(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file.\n";
        return {};
    }

    std::vector<Pallet> pallets;
    std::string line;

    if (!std::getline(file, line)) {
        return pallets;
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        Pallet p;

        if (!std::getline(ss, token, ',')) continue;

        if (!std::getline(ss, token, ',')) continue;
        p.weight = std::stoi(token);

        if (!std::getline(ss, token, ',')) continue;
        p.profit = std::stoi(token);

        pallets.push_back(p);
    }

    return pallets;
}

int parseTruckAndPalletsCSV(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file.\n";
        return -1;
    }

    std::string line;

    if (!std::getline(file, line)) {
        return -1;
    }

    std::getline(file, line);
    std::istringstream ss(line);
    
    int capacity = -1;
    ss >> capacity;
    return capacity;
}
