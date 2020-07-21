#include "benchmark/benchmark.h"

void function_with_large_parameters(uint64_t a, uint64_t b,uint64_t c,
                                    uint64_t d,uint64_t e, uint64_t f, 
                                    uint64_t g) {
    // For comparing apple to apple
    uint64_t result = a + b + c;
}

void function_with_small_parameters(uint64_t a, uint64_t b, uint64_t c,
                                    uint64_t d, uint64_t e, uint64_t f) {
    uint64_t result = a + b + c;
}

struct Arguments {
    int a;
    int b;
    int c;
};

void function_with_arguments_packed_in_struct(struct Arguments& args) {
    int result = args.a + args.b + args.c;
}

void function_with_arguments_not_passed_in_struct(int a, int b, int c) {
    int result = a + b + c;
}

static void BM_function_with_large_parameters(benchmark::State& state) {
    for (auto _ : state) {
        function_with_large_parameters(1,2,3,4,5,6,7);
    }
}

static void BM_function_with_small_parameters(benchmark::State& state) {
    for (auto _ : state) {
        function_with_small_parameters(1,2,3,4,5,6);
    }
}

static void BM_function_with_arguments_packed_in_struct(benchmark::State& state) {
    for (auto _ : state) {
        Arguments args {1,2,3};
        function_with_arguments_packed_in_struct(args);
    }
}

static void BM_function_with_arguments_not_passed_in_struct(benchmark::State& state) {
    for (auto _ : state) {
        function_with_arguments_not_passed_in_struct(1,2,3);
    }
}

class A {
public:
    int x;
    int y;
    virtual int getAdd() {
        return x + y;
    }
};

class B : public A {
};

static void BM_virtual_function_call(benchmark::State& state) {
    B x;
    A* y = &x;
    for (auto _ : state) {
        y->getAdd();
    }
}

static void BM_non_virtual_function_call(benchmark::State& state) {
    A x;
    for (auto _ : state) {
        x.getAdd();
    }
}

BENCHMARK(BM_function_with_small_parameters);
BENCHMARK(BM_function_with_large_parameters);
BENCHMARK(BM_function_with_arguments_packed_in_struct);
BENCHMARK(BM_function_with_arguments_not_passed_in_struct);
BENCHMARK(BM_virtual_function_call);
BENCHMARK(BM_non_virtual_function_call);

BENCHMARK_MAIN();
