#ifndef _BENCHMARK_HPP_
#define _BENCHMARK_HPP_

#include <chrono>

#define BENCHMARK_START auto start = std::chrono::high_resolution_clock::now();
#define BENCHMARK_STOP(name) auto end = std::chrono::high_resolution_clock::now(); \
auto time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); \
std::cout << "[TIME BENCHMARK] " << name << ": " << time_taken  << " ns" << std::endl;
#endif