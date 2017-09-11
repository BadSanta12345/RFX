#include "Refractor.h"

namespace DICE
{
	namespace memory
	{
		HMODULE hMemoryDLL = GetModuleHandleA("Memory.dll");
		// Fortunately dice exported these functions and saved me from some reverse engineering.
		bf_free_t bf_free = (bf_free_t)GetProcAddress(hMemoryDLL, "?bf_free@memory@hfe@dice@@YAXPAX@Z");
		bf_malloc_t bf_malloc = (bf_malloc_t)GetProcAddress(hMemoryDLL, "?bf_malloc@memory@hfe@dice@@YAPAXIH@Z");
		getSizeOfAllocation_t getSizeOfAllocation = (getSizeOfAllocation_t)GetProcAddress(hMemoryDLL, "?getSizeOfAllocation@memory@hfe@dice@@YAHPAX@Z");
	}
}