# LSB-Search-Benchmark

**LSB-Search-Benchmark** is a C++ benchmarking suite that compares three methods for finding the least-significant set bit (LSB) in a 64-bit integer. The three methods are:

1. **De Bruijn Sequence Lookup Method**  
   Uses a precomputed lookup table combined with a magic constant to achieve constant-time (O(1)) performance.

2. **Iterative Method**  
   Iterates through each bit until it finds the set bit. This approach is simple but can be slower, particularly in worst-case scenarios.

3. **Compiler Intrinsics Method**  
   Utilizes the built-in compiler function `__builtin_ctzll` (available in GCC and Clang) to count trailing zeros. This highly optimized intrinsic often outperforms manual implementations.

## Overview

The project performs three main tests:

1. **Correctness Test:**  
   Validates that all three methods return the expected result for a set of known bit positions (e.g., positions 0, 1, 11, 31, and 63).

2. **Random Performance Test (Uniform Distribution):**  
   Benchmarks all three methods on 10 million uniformly random 64-bit numbers (each with at least one bit set). This test calculates the average execution time and the standard deviation over multiple trials.
   - **Average Time:** The mean execution time (in microseconds) over several trials.
   - **Standard Deviation (Std Dev):** Measures how much the execution times vary from the average. A low standard deviation indicates consistent performance across trials, while a higher value suggests more variability.

3. **Worst-Case Performance Test:**  
   Benchmarks all three methods on 10 million numbers where only the highest bit (bit 63) is set. This scenario is particularly challenging for the iterative method, as it must check all 64 bits, whereas the De Bruijn and Compiler Intrinsics methods maintain constant-time performance.

In the performance tests, an accumulated sum (declared as `volatile`) is used to combine the results from each method. This prevents the compiler from optimizing away the function calls during benchmarking.

## Executables

This project builds two separate executables:

- **LSB-Search-Benchmark-Single:**  
  Runs single-threaded performance tests including correctness, random performance, and worst-case performance tests.

- **LSB-Search-Benchmark-Multi:**  
  Runs multi-threaded performance tests. In this version, the dataset is partitioned among multiple threads (using C++11 threads) to evaluate parallel performance. The multi-threaded tests report overall average execution times and standard deviations.

## Requirements

- A C++ compiler with C++17 support (e.g., g++ 7.0 or later).
- GNU Make (or any compatible make tool).

## Project Structure



```
comparison_project/
├── Makefile
├── include/
│   ├── debruijn.hpp
│   └── iterative.hpp
├── obj/           # Object files will be placed here.
└── src/
    ├── debruijn.cpp
    ├── iterative.cpp
    └── main.cpp
```

## How to Build and Run

1. **Build the Project:**  
   Open a terminal in the project's root directory and run:
   ```bash
   make
2. **Run the Tests:**
   After building, execute the benchmark tests by running:
   ```bash
   make run
3. **Clean Up Build Files:**
   To remove the generated object files and the executable, run:
   ```bash
   make clean