// benchmark.cpp
#include "benchmark.h"            // parsePalletsCSV, parseTruckAndPalletsCSV, BenchmarkResult
#include <vector>
#include <string>
#include <future>
#include <thread>
#include <chrono>
#include <fstream>

/**
 * @brief Runs a function that returns a double (elapsed time),
 *        but aborts and returns -1.0 if it takes longer than timeoutSec.
 *
 * The lambda you pass in should capture everything it needs by value.
 */
template<typename Fn>
double timeWithTimeout(Fn&& func, double timeoutSec) {
    // package the work into a task returning double
    std::packaged_task<double()> task{std::forward<Fn>(func)};
    auto fut = task.get_future();

    // launch on a detached thread
    std::thread(std::move(task)).detach();

    // wait up to timeoutSec
    if (fut.wait_for(std::chrono::duration<double>(timeoutSec))
        == std::future_status::ready) {
        return fut.get();    // got the real timing
    } else {
        return -1.0;         // timed out
    }
}

void runBenchmarks() {
    std::vector<BenchmarkResult> results;
    const std::vector<std::string> algorithmNames = {
        "Exhaustive Search", "Backtracking", "Dynamic Programming", "Approximation", "ILP"
    };

    // Loop datasets 1–14
    for (int dataset = 1; dataset <= 10; ++dataset) {
        // build zero-padded dataset string
        std::string ds = (dataset < 10 ? "0" : "") + std::to_string(dataset);
        std::string pathP = "../data/Pallets_"          + ds + ".csv";
        std::string pathT = "../data/TruckAndPallets_" + ds + ".csv";

        // parse outside the threads
        auto pallets = parsePalletsCSV(pathP);
        int capacity = parseTruckAndPalletsCSV(pathT);

        // Test each of the 5 algorithms, up to 2 seconds each
        for (int algo = 1; algo <= 5; ++algo) {
            // Everything the lambda needs is captured by value:
            //   - a copy of pallets
            //   - the capacity
            //   - the algorithm index
            auto timedCall = [pallets, capacity, algo]() -> double {
                auto t0 = std::chrono::high_resolution_clock::now();
                switch (algo) {
                    case 1: exhaustiveSearch(      pallets, capacity); break;
                    case 2: backtracking(           pallets, capacity); break;
                    case 3: dynamicProgramming(     pallets, capacity); break;
                    case 4: approximationAlgorithm( pallets, capacity); break;
                    case 5: integerLinearProgramming(pallets, capacity); break;
                }
                auto t1 = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double>(t1 - t0).count();
            };

            // run it, but give up after 2 seconds
            double elapsed = timeWithTimeout(timedCall, 2.0);
            results.push_back({ algorithmNames[algo-1], dataset, elapsed });
        }
    }

    // Write all results to CSV
    std::ofstream csv("benchmark.csv");
    csv << "Algorithm,Dataset,Time(sec)\n";
    for (auto& r : results) {
        csv << r.algorithm
            << ',' << r.dataset
            << ',' << r.time_seconds
            << '\n';
    }
}