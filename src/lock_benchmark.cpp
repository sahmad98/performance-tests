#include <benchmark/benchmark.h>
#include <atomic>

static uint64_t lock_underlying = 0;

class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};

static SpinLock spinlock;

void lock(uint64_t* underlying) {
    do {} while(not __sync_bool_compare_and_swap(underlying, 0, 1));
}

void unlock(uint64_t* underlying) {
    do {} while(not __sync_bool_compare_and_swap(underlying, 1, 0));
}

void readModifyWrite(int* num) {
    lock(&lock_underlying);
    for (int i=0; i<1000; i++) {
        *num = *num + 1;
    }
    unlock(&lock_underlying);
}

void readModifyWriteGccLock(int *num) {
    spinlock.lock();
    for (int i=0; i<1000; i++) {
        *num = *num + 1;
    }
    spinlock.unlock();
}

static void BM_custom_lock(benchmark::State& state) {
    int n = 0;
    for (auto _ : state) {
        readModifyWrite(&n);
    }
}

static void BM_gcc_lock(benchmark::State& state) {
    int n = 0;
    for (auto _ : state) {
        readModifyWriteGccLock(&n);
    }
}

BENCHMARK(BM_custom_lock);
BENCHMARK(BM_gcc_lock);

BENCHMARK_MAIN();
