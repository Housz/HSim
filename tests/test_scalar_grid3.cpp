#include <HSim/scalar_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/vertex_center_scalar_grid3.h>

int main()
{
	size_t n = 100;

	std::vector<float> data(n * n * n, 1);

	// HSim::CellCenterScalarGrid3<float> sg3({n, n, n});
	HSim::VertexCenterScalarGrid3<float> sg3({n, n, n});

	sg3.setData(data);

	std::cout << sg3._gridSpacing << sg3._girdOrigin << sg3._gridResolution << std::endl;

	std::cout << sg3._data[0] << std::endl;
	sg3(1, 2, 3) = 2;
	std::cout << sg3(1, 2, 3) << std::endl;
	std::cout << sg3.gridOrigin() << std::endl;
	std::cout << sg3.gridResolution() << std::endl;

	// auto func = [&](size_t i, size_t j, size_t k)
	// {
	// 	sg3(i, j, k) = i + j*sg3.sizeX() + k*sg3.sizeY()*sg3.sizeX();
	// };

	// sg3.parallelForEachCell(func);
	// sg3.forEachCell(func);

	// std::cout << sg3(0, 0, 0) << std::endl;
	// std::cout << sg3(n-1, 0, 0) << std::endl;
	// std::cout << sg3(1, 0, 0) << std::endl;
	// std::cout << sg3(0, 1, 0) << std::endl;
	// std::cout << sg3(0, 0, 1) << std::endl;
	// std::cout << sg3(1, 1, 1) << std::endl;

	auto func_laplacian = [&](size_t i, size_t j, size_t k)
	{
		auto lapl = sg3.laplacianAt(i, j, k);
	};

	auto func_gradient = [&](size_t i, size_t j, size_t k)
	{
		auto grad = sg3.gradientAt(i, j, k);
	};

	sg3.forEachCell(func_laplacian);
	sg3.forEachCell(func_gradient);

	sg3.fill(3);
	std::cout << sg3(99, 99, 99);

	std::cout << sg3.dataSize();
	std::cout << sg3.dataOrigin();
	

	return 0;
}