#include <benchmark/benchmark.h>
#include <omp.h>
#include <cstdlib>
#include <cstring>

namespace valgo {
    void memcpy(char* dst, char const * src, std::size_t n) {
#pragma omp simd simdlen(16)
        for (int i = 0; i < n; i++) {
            dst[i] = src[i];
        }
    }
}

void RandomData(char* data, std::size_t n) {
    for (int i=0 ; i<n; i++) {
        data[i] = rand() % 255;
    }
}

constexpr int KiloByte() {
    return 1024;
} 

class MemcpyFixture : public ::benchmark::Fixture {
    void SetUp(const ::benchmark::State& state) override {
        size = state.range(0) * KiloByte();
        dest = new char[size];
        src = new char[size];

        RandomData(src, size);
    }

    void TearDown(const ::benchmark::State& state) override {
        delete[] dest;
        delete[] src;
    }

public:
    char* dest = nullptr;
    char* src = nullptr;
    int size = 0;
};

BENCHMARK_DEFINE_F(MemcpyFixture, StdlibMemcpy)(benchmark::State& st) {
    for (auto _ : st) {
        std::memcpy(dest, src, size);
    }
}

BENCHMARK_DEFINE_F(MemcpyFixture, VectorizedMemcpy)(benchmark::State& st) {
    for (auto _ : st) {
        valgo::memcpy(dest, src, size);
    }
}

#define RUN_FOR_ALL_SIZE(f, t) BENCHMARK_REGISTER_F(f, t)->RangeMultiplier(2)->Range(1, 1 << 10);

//RUN_FOR_ALL_SIZE(MemcpyFixture, StdlibMemcpy);
//RUN_FOR_ALL_SIZE(MemcpyFixture, VectorizedMemcpy);

BENCHMARK_REGISTER_F(MemcpyFixture, StdlibMemcpy)->Args({8 * 1024});

BENCHMARK_MAIN();
