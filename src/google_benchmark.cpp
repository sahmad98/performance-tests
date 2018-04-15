#include "benchmark/benchmark.h"

static void BM_string_creation(benchmark::State& state) {
	for (auto _ : state)
		std::string empty_string;
}

BENCHMARK(BM_string_creation);

static void BM_string_copy(benchmark::State& state) {
	std::string x = "hello";
	for (auto _ : state)
		std::string copy(x);
}

BENCHMARK(BM_string_copy);

BENCHMARK_MAIN();
