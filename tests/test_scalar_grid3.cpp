#include <HSim/scalar_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>

int main()
{
	size_t n = 100;

	std::vector<float> data(n * n * n, 1);

	HSim::ScalarGrid3<float> sg3(n, n, n);

	sg3.setData(data);

	std::cout << sg3._data[0] << std::endl;
	sg3(1, 2, 3) = 2;
	std::cout << sg3(1, 2, 3) << std::endl;
	std::cout << sg3.origin() << std::endl;
	std::cout << sg3.resolution() << std::endl;

	auto& func = [&](size_t i, size_t j, size_t k)
	{
		sg3(i, j, k) = 3;
	};

	sg3.parallelForEachCell(func);
	sg3.forEachCell(func);

	std::cout << sg3(1, 2, 3) << std::endl;


	// sg3.parallelForEachCell([&](size_t i, size_t j, size_t k)
	// {
	// 	sg3(i, j, k) = 3;
	// });

	return 0;
}