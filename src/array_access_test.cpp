#include <iostream>
#include <chrono>
#include "benchmark.hpp"

using namespace std;
#define ARRARY_SIZE 4096

// Cache hits will occur in this case
void row_wise_access(int** array) {
    int result = 0;
    for(int i=0; i<ARRARY_SIZE; i++)
        for(int j=0; j<ARRARY_SIZE; j++) {
            result += array[i][j];
        }
}

// Cache miss will occur in this case
void column_wise_access(int** array) {
    int result = 0;
    for(int i=0; i<ARRARY_SIZE; i++)
        for(int j=0; j<ARRARY_SIZE; j++) {
            result += array[j][i];
        }
}

int main() {
    int** array = new int*[ARRARY_SIZE];
    for (int i =0; i<ARRARY_SIZE; i++) {
        array[i] = new int[ARRARY_SIZE];
    }

    {
        BENCHMARK_START;
        row_wise_access(array);
        BENCHMARK_STOP("ROW WISE ACCESS");
    }
    
    {
        BENCHMARK_START;
        column_wise_access(array);
        BENCHMARK_STOP("COLUMN WISE ACCESS");
    }
}


