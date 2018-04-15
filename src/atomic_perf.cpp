#include "benchmark/benchmark.h"
#include <atomic>

struct NonAtomic {
    int a;
};

struct Atomic {
    std::atomic<int> a;
};

static void BM_normal_fetch_add(benchmark::State& state) {
    NonAtomic data;
    for (auto _ : state) {
        data.a += 10;
        benchmark::ClobberMemory();
    }
}

static void BM_atomic_fetch_add(benchmark::State& state) {
    Atomic data;
    for (auto _ : state) {
        data.a.fetch_add(10);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_normal_fetch_add);
BENCHMARK(BM_atomic_fetch_add);
BENCHMARK_MAIN();