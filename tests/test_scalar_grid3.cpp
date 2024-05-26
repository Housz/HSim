#include <HSim/scalar_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/vertex_center_scalar_grid3.h>

// #define TEST
#ifdef TEST

int main()
{
	size_t n = 100;

	std::vector<float> data(n * n * n, 1);

	// HSim::CellCenterScalarGrid3<float> sg3({n, n, n});
	HSim::VertexCenterScalarGrid3<float> sg3({n, n, n});

	sg3.setData(data);

	std::cout << sg3.gridSpacing << sg3.gridOrigin << sg3.gridResolution << std::endl;

	std::cout << sg3._data[0] << std::endl;
	sg3(1, 2, 3) = 2;
	std::cout << sg3(1, 2, 3) << std::endl;
	std::cout << sg3.gridOrigin << std::endl;
	std::cout << sg3.gridResolution << std::endl;

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

#endif // TEST

// #define POSITION_AT
#ifdef POSITION_AT

int main()
{
	const size_t n = 100;

	HSim::Vec3i gridOrigin = {-1, -1, -1};
	HSim::Vec3i gridSpacing = {1, 2, 3};

	HSim::CellCenterScalarGrid3<float> cellGrid({n, n, n}, gridOrigin, gridSpacing);
	HSim::VertexCenterScalarGrid3<float> vertGrid({n, n, n}, gridOrigin, gridSpacing);

	std::cout << cellGrid.positionAt(0, 0, 0);
	std::cout << vertGrid.positionAt(0, 0, 0);
	std::cout << std::endl;
	std::cout << cellGrid.positionAt(1, 1, 1);
	std::cout << vertGrid.positionAt(1, 1, 1);

	return 0;
}

#endif // POSITION_AT

#define INTERPOLATION

#ifdef INTERPOLATION

int main()
{
	const size_t n = 100;

	HSim::Vec3i gridOrigin = {-1, -1, -1};
	HSim::Vec3i gridSpacing = {1, 2, 3};

	// HSim::CellCenterScalarGrid3<float> cellGrid({n, n, n}, gridOrigin, gridSpacing);
	// HSim::VertexCenterScalarGrid3<float> vertGrid({n, n, n}, gridOrigin, gridSpacing);
	HSim::CellCenterScalarGrid3<float> cellGrid(n, n, n);
	HSim::VertexCenterScalarGrid3<float> vertGrid(n, n, n);

	// cellGrid.fill(1);
	// vertGrid.fill(1);
	cellGrid.parallelForEachCell([&](size_t i, size_t j, size_t k){
		cellGrid(i, j, k) = i;
		vertGrid(i, j, k) = i; 
	});

	// std::cout << vertGrid.dataAt(1, 1, 1) << std::endl;
	// std::cout << vertGrid.positionAt(1, 1, 1) << std::endl;
	std::cout << vertGrid.sample({1, 1, 1}) << std::endl;

	std::cout << cellGrid.sample({1, 1, 1}) << std::endl;

}

#endif // INTERPOLATION