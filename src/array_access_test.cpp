#include <iostream>
#include <chrono>
#include <benchmark/benchmark.h>

using namespace std;

int** allocateArray(uint64_t size) {
    int** array = new int*[size];
    for (int i =0; i<size; i++) {
        array[i] = new int[size];
    }
    return array;
}

//Cache hit
void row_wise_access(int** array, int size) {
    int result = 0;
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++) {
            result += array[i][j];
        }
}

void deallocateArray(int** arr, int size) {
    for (int i =0; i<size; i++) {
        auto temp = arr[i];
        delete[] temp;
    }
}

//Cache miss
void column_wise_access(int** array, int size) {
    int result = 0;
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++) {
            result += array[j][i];
        }
}

static void BM_row_wise_access(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        int size = state.range(0);
        int** arr = allocateArray(size);
        state.ResumeTiming();
        row_wise_access(arr, size);
        state.PauseTiming();
        deallocateArray(arr, size);
        state.ResumeTiming();
    }	
}

static void BM_column_wise_access(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        int size = state.range(0);
        int** arr = allocateArray(size);
        state.ResumeTiming();
        column_wise_access(arr, size);
        state.PauseTiming();
        deallocateArray(arr, size);
        state.ResumeTiming();
    }	
}

BENCHMARK(BM_row_wise_access)->Range(8, 8 << 10);
BENCHMARK(BM_column_wise_access)->Range(8, 8 << 10);

BENCHMARK_MAIN();
