#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cstdint>
#include <numeric>
#include <cmath>
#include <iomanip>

#include "debruijn.hpp"
#include "iterative.hpp"

//------------------------------------------------------------------------------
// Scientific Comparison of LSB Search Algorithms
//
// We perform three tests:
// 1. Correctness Test: Validate both methods on known bit positions.
// 2. Random Performance Test: Benchmark both methods on uniformly random numbers.
// 3. Worst-Case Performance Test: Benchmark both methods when the LSB is at bit 63.
//
// The performance tests are repeated for several trials, and the average and
// standard deviation of execution times are computed.
//------------------------------------------------------------------------------

// Structure to hold statistics.
struct Stats
{
    double avg;    // average time in microseconds
    double stddev; // standard deviation in microseconds
};

// Compute average and standard deviation for a set of timings.
Stats computeStats(const std::vector<double> &times)
{
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double avg = sum / times.size();
    double sq_sum = 0.0;
    for (double t : times)
    {
        sq_sum += (t - avg) * (t - avg);
    }
    double stddev = std::sqrt(sq_sum / times.size());
    return {avg, stddev};
}

// Template function to run a performance test on a given dataset using the provided
// LSB function (either debruijn::getLSBIndex or iterative::getLSBIndex) for a specified
// number of trials. It returns the computed average time and standard deviation.
template <typename Func>
Stats runPerformanceTest(const std::vector<uint64_t> &data, Func func, int trials)
{
    std::vector<double> durations;
    for (int t = 0; t < trials; t++)
    {
        volatile int sum = 0; // Prevent compiler optimization.
        auto start = std::chrono::high_resolution_clock::now();
        for (uint64_t num : data)
        {
            sum += func(num);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        durations.push_back(duration);
    }
    return computeStats(durations);
}

//------------------------------------------------------------------------------
// Test 1: Correctness Test
//------------------------------------------------------------------------------
void testCorrectness()
{
    std::cout << "==== Correctness Test ====\n";
    // Test a few known positions.
    std::vector<int> positions = {0, 1, 11, 31, 63};
    for (int pos : positions)
    {
        uint64_t testNum = 1ULL << pos;
        int resultDeBruijn = debruijn::getLSBIndex(testNum);
        int resultIterative = iterative::getLSBIndex(testNum);
        std::cout << "Bit position: " << std::setw(2) << pos
                  << " | De Bruijn: " << resultDeBruijn
                  << " | Iterative: " << resultIterative << "\n";
    }
    std::cout << "\n";
}

//------------------------------------------------------------------------------
// Test 2: Random Performance Test (Uniform Distribution)
//------------------------------------------------------------------------------
void testRandomPerformance()
{
    std::cout << "==== Random Performance Test (Uniform Distribution) ====\n";
    const size_t numTests = 10000000;
    std::vector<uint64_t> data;
    data.reserve(numTests);

    // Use a fixed seed for reproducibility.
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);
    for (size_t i = 0; i < numTests; i++)
    {
        data.push_back(dist(rng));
    }

    const int trials = 5;
    Stats statsDeBruijn = runPerformanceTest(data, debruijn::getLSBIndex, trials);
    Stats statsIterative = runPerformanceTest(data, iterative::getLSBIndex, trials);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "De Bruijn method:  Average = " << statsDeBruijn.avg
              << " us, Std Dev = " << statsDeBruijn.stddev << " us\n";
    std::cout << "Iterative method:  Average = " << statsIterative.avg
              << " us, Std Dev = " << statsIterative.stddev << " us\n\n";
}

//------------------------------------------------------------------------------
// Test 3: Worst-Case Performance Test (LSB at Bit 63)
//------------------------------------------------------------------------------
void testWorstCasePerformance()
{
    std::cout << "==== Worst-Case Performance Test (LSB at Bit 63) ====\n";
    const size_t numTests = 10000000;
    // Create a dataset where every number is 1ULL << 63.
    std::vector<uint64_t> data(numTests, 1ULL << 63);

    const int trials = 5;
    Stats statsDeBruijn = runPerformanceTest(data, debruijn::getLSBIndex, trials);
    Stats statsIterative = runPerformanceTest(data, iterative::getLSBIndex, trials);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "De Bruijn method:  Average = " << statsDeBruijn.avg
              << " us, Std Dev = " << statsDeBruijn.stddev << " us\n";
    std::cout << "Iterative method:  Average = " << statsIterative.avg
              << " us, Std Dev = " << statsIterative.stddev << " us\n\n";
}

//------------------------------------------------------------------------------
// Main Function: Run all tests sequentially.
//------------------------------------------------------------------------------
int main()
{
    // Initialize the De Bruijn lookup table.
    debruijn::initDeBruijnTable();

    std::cout << "Scientific Comparison of LSB Search Algorithms\n";
    std::cout << "-------------------------------------------------\n\n";

    testCorrectness();
    testRandomPerformance();
    testWorstCasePerformance();

    return 0;
}
