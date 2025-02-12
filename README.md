# LSB-Search-Benchmark

**LSB-Search-Benchmark** is a C++ benchmarking suite that compares two methods for finding the least-significant set bit (LSB) in a 64-bit integer. The two methods are:

1. **De Bruijn Sequence Lookup Method**  
   This method uses a precomputed lookup table combined with a magic constant, resulting in a constant-time (O(1)) solution.

2. **Iterative Method**  
   This classic approach iterates through each bit until it finds the set bit, which can be slower, especially in worst-case scenarios.

## Overview

The project performs three main tests:

1. **Correctness Test:**  
   Validates that both methods return the expected result for a set of known bit positions (e.g., positions 0, 1, 11, 31, and 63).

2. **Random Performance Test (Uniform Distribution):**  
   Benchmarks both methods on 10 million uniformly random 64-bit numbers (with at least one bit set). This test provides an average execution time and the standard deviation over multiple trials.  
   - **Average Time:** Represents the mean execution time (in microseconds) over several trials.  
   - **Standard Deviation (Std Dev):** Indicates how spread out the execution times are around the average. A low standard deviation means that the execution times were very consistent, whereas a higher standard deviation suggests more variability in the timing results.

3. **Worst-Case Performance Test:**  
   Benchmarks both methods on 10 million numbers where only the highest bit (bit 63) is set. This is a worst-case scenario for the iterative method, as it must check all 64 bits, while the De Bruijn method still operates in constant time.

In the performance tests, an accumulated sum (declared as `volatile`) is used to combine the results. This ensures that the compiler does not optimize away the function calls during benchmarking.

## Requirements

- A C++ compiler with C++17 support (e.g., g++ 7.0 or later).
- GNU Make (or any compatible make tool).

## Project Structure

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