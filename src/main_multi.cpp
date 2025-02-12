#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <random>
#include <cstdint>
#include <numeric>
#include <cmath>
#include <iomanip>

#include "debruijn.hpp"
#include "iterative.hpp"
#include "intrinsics.hpp"

// Structure to hold performance statistics.
struct Stats
{
    double avg;    // Average time in microseconds
    double stddev; // Standard deviation in microseconds
};

// Compute average and standard deviation from a vector of timings.
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

// Template function to run a multi-threaded performance test.
// The data is partitioned among 'numThreads' threads.
template <typename Func>
Stats runMultiThreadedPerformanceTest(const std::vector<uint64_t> &data, Func func, int trials, unsigned numThreads)
{
    std::vector<double> durations;
    size_t dataSize = data.size();

    for (int t = 0; t < trials; t++)
    {
        std::vector<int> threadSums(numThreads, 0);
        std::vector<std::thread> threads;
        size_t chunkSize = dataSize / numThreads;

        auto start = std::chrono::high_resolution_clock::now();

        for (unsigned i = 0; i < numThreads; i++)
        {
            size_t startIndex = i * chunkSize;
            size_t endIndex = (i == numThreads - 1) ? dataSize : (i + 1) * chunkSize;
            threads.push_back(std::thread([&, i, startIndex, endIndex]()
                                          {
                int localSum = 0;
                for (size_t j = startIndex; j < endIndex; j++) {
                    localSum += func(data[j]);
                }
                threadSums[i] = localSum; }));
        }

        for (auto &th : threads)
        {
            th.join();
        }

        volatile int sum = 0;
        for (int s : threadSums)
        {
            sum += s;
        }

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        durations.push_back(duration);
    }

    return computeStats(durations);
}

// Multi-threaded Random Performance Test.
void testRandomPerformanceMulti(unsigned numThreads)
{
    std::cout << "==== Multi-Threaded Random Performance Test (Uniform Distribution) ====\n";
    const size_t numTests = 10000000;
    std::vector<uint64_t> data;
    data.reserve(numTests);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);
    for (size_t i = 0; i < numTests; i++)
    {
        data.push_back(dist(rng));
    }

    const int trials = 5;
    Stats statsDeBruijn = runMultiThreadedPerformanceTest(data, debruijn::getLSBIndex, trials, numThreads);
    Stats statsIterative = runMultiThreadedPerformanceTest(data, iterative::getLSBIndex, trials, numThreads);
    Stats statsIntrinsics = runMultiThreadedPerformanceTest(data, intrinsics::getLSBIndex, trials, numThreads);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "De Bruijn:  Avg = " << statsDeBruijn.avg << " us, Std Dev = " << statsDeBruijn.stddev << " us\n";
    std::cout << "Iterative:  Avg = " << statsIterative.avg << " us, Std Dev = " << statsIterative.stddev << " us\n";
    std::cout << "Intrinsics: Avg = " << statsIntrinsics.avg << " us, Std Dev = " << statsIntrinsics.stddev << " us\n\n";
}

// Multi-threaded Worst-Case Performance Test (LSB at Bit 63).
void testWorstCasePerformanceMulti(unsigned numThreads)
{
    std::cout << "==== Multi-Threaded Worst-Case Performance Test (LSB at Bit 63) ====\n";
    const size_t numTests = 10000000;
    std::vector<uint64_t> data(numTests, 1ULL << 63);

    const int trials = 5;
    Stats statsDeBruijn = runMultiThreadedPerformanceTest(data, debruijn::getLSBIndex, trials, numThreads);
    Stats statsIterative = runMultiThreadedPerformanceTest(data, iterative::getLSBIndex, trials, numThreads);
    Stats statsIntrinsics = runMultiThreadedPerformanceTest(data, intrinsics::getLSBIndex, trials, numThreads);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "De Bruijn:  Avg = " << statsDeBruijn.avg << " us, Std Dev = " << statsDeBruijn.stddev << " us\n";
    std::cout << "Iterative:  Avg = " << statsIterative.avg << " us, Std Dev = " << statsIterative.stddev << " us\n";
    std::cout << "Intrinsics: Avg = " << statsIntrinsics.avg << " us, Std Dev = " << statsIntrinsics.stddev << " us\n\n";
}

int main()
{
    // Initialize the De Bruijn lookup table.
    debruijn::initDeBruijnTable();

    // Determine the number of threads to use.
    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0)
    { // Fallback if hardware_concurrency() cannot determine.
        numThreads = 4;
    }

    std::cout << "Multi-Threaded Performance Tests for LSB Search Algorithms\n";
    std::cout << "Using " << numThreads << " threads.\n";
    std::cout << "-----------------------------------------------------------\n\n";

    testRandomPerformanceMulti(numThreads);
    testWorstCasePerformanceMulti(numThreads);

    return 0;
}
