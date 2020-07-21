#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>

using namespace std;

struct UnaligendAtomic {
	std::atomic<int> x{0};
	std::atomic<int> y{0};
};

struct AligendAtomic {
  alignas(64) std::atomic<int> x{0};
  alignas(64) std::atomic<int> y{0};
};

static std::atomic<bool> wait{true};
static std::atomic<bool> x_started{false};
static std::atomic<bool> y_started{false};

template<typename T>
void ModifyX(T& data) {
  std::cout << "X Waiting" << std::endl;
  x_started = true;
	while(wait) {}
	for (int i=0; i<100000000;i++) {
		data.x = data.x + 1;
	}
}

template<typename T>
void ModifyY(T& data) {
  y_started = true;
  std::cout << "Y Waiting" << std::endl;
	while(wait) {}
	for (int i=0; i<100000000;i++) {
		data.y = data.y + 1;
	}
}

int main() {
  using CacheStruct = UnaligendAtomic;
  CacheStruct data;
  std::cout << "Starting Threads" << std::endl;
  std::thread t1{ModifyX<CacheStruct>, std::ref(data)};
  std::thread t2{ModifyY<CacheStruct>, std::ref(data)};
  // Setting it to different cores
  cpu_set_t cpu3;
  cpu_set_t cpu1;
  CPU_ZERO(&cpu3);
  CPU_ZERO(&cpu1);
  CPU_SET(3, &cpu3);
  CPU_SET(1, &cpu1);

  pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t), &cpu1);
  pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t), &cpu3);

  while(!x_started) {}
  while(!y_started) {}

  wait = false;
  std::cout << "Signalled" << std::endl;
  t1.join();
  t2.join();
}
