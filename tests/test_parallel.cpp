#include <vector>
#include <iostream>
#include <algorithm>
#include <HSim/parallel.h>
#include <HSim/vec.h>

// #define TEMP

#define TEST

int main()
{

#ifdef TEMP
	// size_t n = 1 << 26;
	size_t n = 100;
	std::vector<float> v(n);

	tbb::parallel_for((size_t)0, (size_t)n, [&] (size_t i) {
		v[i] = i;
	});


	// HSim::parallelFill(v.begin(), v.end(), 2);

	
	HSim::parallelFor(v.begin(), v.end(), [&](auto &i) {
		i = 2;
	});
	HSim::parallelFor(v.begin(), v.end(), [&](auto i) {
		// i = 2;
		std::cout << i << std::endl;
	});
	
#endif

#ifdef TEST

	size_t n = 1 << 30;
	// size_t n = 100;

	HSim::Vecd vd(n);

	vd.set(3);

	// std::fill(vd.begin(), vd.end(), 2);

	// std::cout << vd << std::endl;


#endif 

	return 0;
}