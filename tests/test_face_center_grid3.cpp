#include <HSim/face_center_grid3.h>

// #define TEST
#ifdef TEST
int main()
{
	size_t n = 100;
	HSim::FaceCenterGrid3<float> fcg(n, n, n);

	std::cout << fcg.dataSizeU();
	std::cout << fcg.dataSizeV();
	std::cout << fcg.dataSizeW();

	std::cout << fcg.dataOriginU();
	std::cout << fcg.dataOriginV();
	std::cout << fcg.dataOriginW();

	std::cout << fcg.u(100, 99, 99) << std::endl;

	std::cout << fcg.dataAtCellCenter(99, 99, 99);
	std::cout << fcg.divergenceAtCellCenter(99, 99, 99);

	return 0;
}
#endif // TEST

#define SAMPLE
#ifdef SAMPLE
int main()
{
	size_t n = 10;
	HSim::FaceCenterGrid3<float> grid(n, n, n);

	grid.dataU().parallelForEachCell([&](size_t i, size_t j, size_t k)
									 { grid.dataU()(i, j, k) = i; });

	grid.dataV().parallelForEachCell([&](size_t i, size_t j, size_t k)
									 { grid.dataV()(i, j, k) = j; });

	grid.dataW().parallelForEachCell([&](size_t i, size_t j, size_t k)
									 { grid.dataW()(i, j, k) = k; });

	std::cout << grid.sample({.2, .3, .4});
	std::cout << grid.sample({1.5, 1.5, 1.5});

	return 0;
}
#endif // SAMPLE