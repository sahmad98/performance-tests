#include "benchmark/benchmark.h"
#include <atomic>

struct StructFittingInSameCacheLine {
    std::atomic<int> a{0};
    std::atomic<int> b{0};
    std::atomic<int> c{0};
};

struct StructFittingInSeperateCacheLine {
    std::atomic<int> a{0};
    char padding[33000];
    std::atomic<int> b{0};
};

static void SameReadModifyWrite (benchmark::State& state) {
    StructFittingInSameCacheLine temp;
    for (auto _ : state) {
        auto r = temp.a.load();
        temp.b.fetch_add(10);
        auto t = temp.c.load();
    }
}

BENCHMARK(SameReadModifyWrite);

static void DifferentReadModifyWrite (benchmark::State& state) {
    StructFittingInSameCacheLine temp;
    for (auto _ : state) {
        auto r = temp.a.load();
        temp.b.fetch_add(10);
        auto t = temp.c.load();
    }
}

BENCHMARK(DifferentReadModifyWrite);

BENCHMARK_MAIN();