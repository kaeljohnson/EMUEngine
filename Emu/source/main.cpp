#include "../include/EMU.h"
#include "../include/Init.h"

#include <mutex>

static std::unordered_map<void*, size_t> allocationMap;
static std::mutex allocMutex;

static size_t totalAllocated = 0;

std::atomic<int> allocationCount = 0;

void* operator new(std::size_t size) {
	void* ptr = std::malloc(size);
	totalAllocated += size;

	int count = ++allocationCount;

	{
		std::lock_guard<std::mutex> lock(allocMutex);
		std::cout << "Allocated " << size << " bytes (count: " << count << ")\n";
	}

	if (count == 400) {
		std::lock_guard<std::mutex> lock(allocMutex);
		std::cout << "=== 400th allocation reached ===\n";
		// throw std::runtime_error("400th allocation reached");
	}

	return ptr;
}

void operator delete(void* ptr) noexcept {
	std::lock_guard<std::mutex> lock(allocMutex);
	std::cout << "Freed bytes from " << ptr << "\n";
	free(ptr);
}

int main(int argc, char** argv)
{
	Engine::Init();
	Engine::EMU engine(10000);

	engine.RunApp();

	return 0;
}