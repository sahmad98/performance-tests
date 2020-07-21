#include <iostream>
#include <chrono>
#include <benchmark/benchmark.h>
#include <vector>

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
    // loop
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
    // loop
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++) {
            result += array[j][i];
        }
}

static void BM_row_wise_access(benchmark::State& state) {
    int size = state.range(0);
    int** arr = allocateArray(size);
    for (auto _ : state) {
	    row_wise_access(arr, size);
	    benchmark::ClobberMemory();
    }
    deallocateArray(arr, size);
}

static void BM_column_wise_access(benchmark::State& state) {
    int size = state.range(0);
    int** arr = allocateArray(size);
    for (auto _ : state) {
        column_wise_access(arr, size);
    }
    deallocateArray(arr, size);
}

BENCHMARK(BM_row_wise_access)->Range(8, 8 << 10);
BENCHMARK(BM_column_wise_access)->Range(8, 8 << 10);

BENCHMARK_MAIN();
