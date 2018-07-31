#include <iostream>
#include <chrono>
#include <benchmark/benchmark.h>

class CustomInterface {
public:
  virtual void calculations() = 0;
};

class CRTPInterface {
public:
  void calculations() {}
};

template<typename T>
class BaseClass : public CRTPInterface {
public:
  void calculations() {
    static_cast<T*>(this)->calculations();
  }
};

class DerivedClass : public BaseClass<DerivedClass> {
public:
  void calculations() {
    int x = 0;
  }
};

class DynamicVirutalClass : public CustomInterface {
public:
  virtual void calculations() {
    int x = 0;
  }
};

static void BM_CRTP_Virtual_Method_Call(benchmark::State& state) {
  CRTPInterface* object = new DerivedClass();
  for ( auto _ : state ) {
    object->calculations();
  }
}

static void BM_Virtual_Method_Call(benchmark::State& state) {
  CustomInterface* object = new DynamicVirutalClass();
  for ( auto _ : state ) {
    object->calculations();
  }
}

BENCHMARK(BM_CRTP_Virtual_Method_Call);
BENCHMARK(BM_Virtual_Method_Call);

BENCHMARK_MAIN();
