#include <HSim/face_center_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/vertex_center_scalar_grid3.h>

HSim::Size3 gridResolution = {32, 32, 32};

bool isBoundary(std::shared_ptr<HSim::FaceCenterGrid3<float>> grid, size_t x, size_t y, size_t z)
{
	if (x <= 0 || y <= 0 || z <= 0 ||
		x >= grid->sizeX() - 1 || y >= grid->sizeY() - 1 || z >= grid->sizeZ() - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double residual(std::shared_ptr<HSim::FaceCenterGrid3<double>> grid)
{
	double res = 0;

	grid->forEachCell([&](size_t i, size_t j, size_t k){
		
		auto div = grid->divergenceAtCellCenter(i, j, k);

		res += div * div;

	});

	return res;
}

int main()
{
	auto MACGrid = std::make_shared<HSim::FaceCenterGrid3<double>>(gridResolution);

	// init
	for (size_t i = 10; i < 13; i++)
	{
		for (size_t j = 10; j < 13; j++)
		{
			for (size_t k = 10; k < 13; k++)
			{
				MACGrid->u(i, j, k) = 1;
				MACGrid->v(i, j, k) = 1;
				MACGrid->v(i, j, k) = 1;
			}
		}
	}

	auto MACGrid_old = std::make_shared<HSim::FaceCenterGrid3<float>>(gridResolution);

	std::cout << residual(MACGrid) << std::endl;
	


	return 0;
}