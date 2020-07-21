#include <benchmark/benchmark.h>
#include <array>
#include <iostream>

struct CircularBuffer {
  std::array<int, 16> data{};

  void fill(std::array<int, 16> in) {
    data = in;
  }

  void init(int d) {
    data[0] = d;
  }
};

class TestClass : public ::benchmark::Fixture {
public:
  virtual void SetUp(::benchmark::State& state) {
    buffer.init(10);
    std::cout << "Setup Called" << std::endl;
  }

  virtual void TearDown(::benchmark::State& state) {
    std::cout << "TearDown Called" << std::endl;
  }

  CircularBuffer buffer;
};

BENCHMARK_F(TestClass, FillTest)(::benchmark::State& state) {
  std::array<int, 16> d{12};
  for (auto _ : state) {
    buffer.fill(d);
  }
}

BENCHMARK_MAIN();
