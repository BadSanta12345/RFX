#pragma once
#include <chrono>
#include <iostream>

/*Simple profiling macro*/

#define PROFILE(NAME)bs::profiler p(NAME);

namespace bs
{
	class profiler
	{
	public:
		profiler(const char* name) : name(name) {}
		~profiler()
		{
			std::cout << name << ": " << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(std::chrono::high_resolution_clock::now() - start).count() << '\n';
		}
		const char* name;
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	};
}