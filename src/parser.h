/**
 * @file parser.h
 * @brief Header file for CSV parsing functions related to pallets and truck data.
 */

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "pallet.h"

/**
 * @brief Parses a CSV file containing pallet information.
 *
 * Expects a file where each line has the format: PalletID,Weight,Profit
 * Skips the header line and converts each line into a Pallet object.
 *
 * @param filePath The path to the CSV file.
 * @return A vector of Pallet objects parsed from the file.
 */
std::vector<Pallet> parsePalletsCSV(std::string filePath);

/**
 * @brief Parses the truck capacity from a CSV file.
 *
 * Expects a file where the second line contains the capacity as the first value.
 * Only this value is extracted; the number of pallets is ignored.
 *
 * @param filePath The path to the CSV file.
 * @return The truck's maximum weight capacity, or -1 on error.
 */
int parseTruckAndPalletsCSV(std::string filePath);

#endif // PARSER_H
