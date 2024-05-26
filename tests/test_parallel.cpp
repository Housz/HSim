#include <vector>
#include <iostream>
#include <algorithm>
#include <HSim/parallel.h>
#include <HSim/vec.h>

// #define TEMP
// #define TEST
#define REDUCE

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

#ifdef REDUCE

	size_t m = 1 < 10;
	size_t n = 1 < 11;
	std::vector<std::vector<float>> v(m, std::vector<float>(n));

	tbb::parallel_for(tbb::blocked_range2d<size_t>(0, n, 0, n),
		[&](tbb::blocked_range2d<size_t> rA){
			for(size_t i = rA.rows().begin(); i < rA.rows().end(); i++)
			{
				for(size_t j = rA.cols().begin(); j < rA.cols().end(); j++)
				{
					double s = HSim::parallelReduce(
						0, n,
						0.,
						[&](tbb::blocked_range<size_t> r, float local_sum)
						{
							for (size_t col = r.begin(); col < r.end(); col++)
							{
								local_sum += v[i][col];
							}
							return local_sum;
						},
						std::plus<float>()
					);
					v[i][j] = s;
				}
			}
		}
	);


#endif

	return 0;
}