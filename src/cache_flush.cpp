//#include <asm/cachectl.h>

struct DataSoA {
	int x[16];
	int y[16];
};

int main() {
	DataSoA* d = new DataSoA{};
	cacheflush(d, sizeof(DataSoA), BCACHE);
	return 0;
}
