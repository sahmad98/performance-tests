#include "xmmintrin.h"
#include "benchmark/benchmark.h"

#define SIZE 8096
float arrayA[SIZE];
float arrayB[SIZE];

void initialise(float *arrA, float *arrB, int size) {
    for (int i=0; i<size; i++) {
        arrA[i] = float(i);
        arrB[size - i - 1] = float(i);
    }
}

float non_vectorized_add(float *arrA, float *arrB, int size) {
    float result = 0.0;
    for (int i=0; i<size; i++) {
        result  = arrA[i] + arrB[i];
    }
    return result;
}

static void BM_non_vectorized_add(benchmark::State& state) {
    initialise(arrayA, arrayB, SIZE);
    for (auto _ : state) {
        non_vectorized_add(arrayA, arrayB, SIZE);
    }
}

float non_vectorized_mul(float *arrA, float *arrB, int size) {
    float result = 0.0;
    for (int i=0; i<size; i++) {
        result  = arrA[i] * arrB[i];
    }
    return result;
}

static void BM_non_vectorized_mul(benchmark::State& state) {
    initialise(arrayA, arrayB, SIZE);
    for (auto _ : state) {
        non_vectorized_mul(arrayA, arrayB, SIZE);
    }
}

__m128 vectorized_add(float *arrA, float *arrB, int size) {
    __m128 result;
    for (int i=0; i<size; i = i+4) {
        __m128 avec = _mm_load_ps(arrA + i);
        __m128 bvec = _mm_load_ps(arrB + i);
        result = _mm_add_ps(avec, bvec);
    }
    return result;
}

static void BM_vectorized_add(benchmark::State& state) {
    initialise(arrayA, arrayB, SIZE);
    for (auto _ : state) {
        vectorized_add(arrayA, arrayB, SIZE);
    }
}

__m128 vectorized_mul(float *arrA, float *arrB, int size) {
    __m128 result;
    for (int i=0; i<size; i = i+4) {
        __m128 avec = _mm_load_ps(arrA + i);
        __m128 bvec = _mm_load_ps(arrB + i);
        result = _mm_mul_ps(avec, bvec);
    }
    return result;
}

static void BM_vectorized_mul(benchmark::State& state) {
    initialise(arrayA, arrayB, SIZE);
    for (auto _ : state) {
        vectorized_mul(arrayA, arrayB, SIZE);
    }
}

BENCHMARK(BM_non_vectorized_add);
BENCHMARK(BM_vectorized_add);
BENCHMARK(BM_non_vectorized_mul);
BENCHMARK(BM_vectorized_mul);
BENCHMARK_MAIN();