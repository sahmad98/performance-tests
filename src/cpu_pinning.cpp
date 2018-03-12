#include <iostream>
#include <thread>
#include <sched.h>
#include <chrono>
#include "benchmark.hpp"

#define ARRARY_SIZE 409600000

void set_and_add(int* arr) {
    std::this_thread::sleep_for(std::chrono::seconds{5});
    {
        BENCHMARK_START;
        int result = 0;
        for (int i =0; i<ARRARY_SIZE; i++) {
            arr[i] = i;
            result += arr[i];
        }
        BENCHMARK_STOP("SETANDADD");
    }
}

int main() {

}