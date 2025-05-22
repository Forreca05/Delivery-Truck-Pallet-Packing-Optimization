#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include "algorithms.h"
#include "parser.h"

struct BenchmarkResult {
    std::string algorithm;
    int dataset;
    double time_seconds;
};

void runBenchmarks();