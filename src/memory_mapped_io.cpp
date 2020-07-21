#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <benchmark/benchmark.h>

static constexpr auto kMaxSize = 8192;
char data[kMaxSize];

static void BM_MemMappedRead(benchmark::State& state) {
	int fd = open("../resources/data.bin", O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		std::cout << "Error in Opening file" << errno << "\n";
	}

	char* map = (char*)mmap(NULL, kMaxSize, PROT_READ, MAP_SHARED, fd, 0);
	if (!map) {
		std::cout << "Error in mapping " << errno << std::endl;
	}

	size_t size = state.range(0);
	for (auto _ : state) {
		std::memcpy(data, map, size);
	}

	close(fd);
	munmap(map, kMaxSize);
}

static void BM_MemMappedWrite(benchmark::State& state) {
	int fd = open("../resources/data_write_map.bin", O_RDWR | O_CLOEXEC);
	if (fd < 0) {
		std::cout << "Error in Opening file " << errno << "\n";
	}

	char* map = (char*)mmap(NULL, kMaxSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (!map) {
		std::cout << "Error in mapping " << errno << std::endl;
	}

	size_t size = state.range(0);
	for (auto _ : state) {
		std::memcpy(map, data, size);
	}

	close(fd);
	munmap(map, kMaxSize);
}


#define min_(x, y) x < y ? x : y

static void BM_ReadSyscall(benchmark::State& state) {
	int fd = open("../resources/data.bin", O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		std::cout << "Error in Opening file" << errno << "\n";
	}

	size_t size = state.range(0);
	for (auto _ : state) {
		int total_read = 0;
		if (lseek(fd, 0, SEEK_SET) < 0) {
			std::cerr << "Error in Seeking " << errno << std::endl;
			return;
		}
		while(total_read < size) {
			size_t read_size = min_(size, size - total_read);
			total_read += read(fd, data + total_read, read_size);
		}
	}

	close(fd);
}

static void BM_WriteSyscall(benchmark::State& state) {
	int fd = open("../resources/data_write_sys.bin", O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		std::cout << "Error in Opening file" << errno << "\n";
	}

	size_t size = state.range(0);
	for (auto _ : state) {
		int total_write = 0;
		if (lseek(fd, 0, SEEK_SET) < 0) {
			std::cerr << "Error in Seeking " << errno << std::endl;
			return;
		}
		while(total_write < size) {
			size_t write_size = min_(size, size - total_write);
			total_write += read(fd, data + total_write, write_size);
		}
	}

	close(fd);
}

BENCHMARK(BM_MemMappedRead)->Range(64, 8 << 10);
BENCHMARK(BM_ReadSyscall)->Range(64, 8 << 10);
BENCHMARK(BM_MemMappedWrite)->Range(64, 8 << 10);
BENCHMARK(BM_WriteSyscall)->Range(64, 8 << 10);
BENCHMARK_MAIN();

