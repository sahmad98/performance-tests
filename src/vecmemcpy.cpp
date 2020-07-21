#include <benchmark/benchmark.h>
#include <omp.h>
#include <cstdlib>
#include <cstring>

namespace valgo {
   __attribute__((always_inline)) void memcpy(char* dst, char const * src, std::size_t n) {
	int alignment = reinterpret_cast<std::uintptr_t>(src) & 0x3F;
	int unaligned_iter = alignment ? 64 - alignment : 0;
	if (unaligned_iter) {
		std::memcpy(dst, src, unaligned_iter);
	}

	const char* new_src = src + unaligned_iter;
	char* new_dst = dst + unaligned_iter;
	int j = unaligned_iter;

	j = unaligned_iter;
	for (; j < n; j = j + 64) {
		__builtin_prefetch(new_src + j + 16384, 0, 0);
		__builtin_prefetch(new_dst + j + 16384, 1, 0);
#pragma omp simd simdlen(8) aligned(new_src: 64)
	        for (int i = 0; i < 64; i++) {
        	    new_dst[j + i] = new_src[j + i];
	        }
	}

	int rem = n - j;
	if (rem > 0) {
		std::memcpy(dst + j, src + j, rem);
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

BENCHMARK_REGISTER_F(MemcpyFixture, VectorizedMemcpy)->Args({8 * 1024});

BENCHMARK_MAIN();
