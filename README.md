# perf_test

## Performance Benchmarking
  - [Array Access Time benchmarking](src/array_access_test.cpp)
  - [Atomic operations benchmarking](src/atomic_perf.cpp)
  - [Thread pinning benchmarking](src/cpu_pinning.cpp)
  - [Isolated CPU benchmarking]() // TODO
  - [Custom spinlock benchmarking](src/lock_benchmark.cpp)
  - [Memory Mapped IO benchmarking](src/memory_mapped_io.cpp)
  - [Vectorised Operations (SIMD) benchmarking](src/vectorized_test.cpp)
  
## Dependencies
  - [google benchmark](https://github.com/google/benchmark)
  - CMake 3.10
  
## Build Setups
  - Clone repository
```bash
  git clone https://github.com/sahmad98/perf_test
  cd perf_test
```
  - Create build directory
```
  mkdir build
  cd build
```
  - Generate Makefile and build
```
  cmake ..
  make
```
