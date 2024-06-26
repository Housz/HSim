#include <HSim/face_center_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/vertex_center_scalar_grid3.h>

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

int main()
{
	HSim::Size3 gridResolution = {32, 32, 32};

	auto MACGrid = std::make_shared<HSim::FaceCenterGrid3<float>>(gridResolution);
	auto MACGrid_old = std::make_shared<HSim::FaceCenterGrid3<float>>(gridResolution);

	auto &u = MACGrid->dataU();
	auto &v = MACGrid->dataV();
	auto &w = MACGrid->dataW();

	auto fillCallbackU = [&](size_t i, size_t j, size_t k)
	{
		u(i, j, k) = i;
	};

	auto fillCallbackV = [&](size_t i, size_t j, size_t k)
	{
		v(i, j, k) = j;
	};

	auto fillCallbackW = [&](size_t i, size_t j, size_t k)
	{
		w(i, j, k) = k;
	};

	u.parallelForEachCell(fillCallbackU);
	v.parallelForEachCell(fillCallbackV);
	w.parallelForEachCell(fillCallbackW);

	std::cout << u(1, 2, 3) << std::endl;
	std::cout << v(1, 2, 3) << std::endl;
	std::cout << w(1, 2, 3) << std::endl;

	std::cout << MACGrid_old->u(1, 2, 3) << std::endl;
	std::cout << MACGrid_old->v(1, 2, 3) << std::endl;
	std::cout << MACGrid_old->w(1, 2, 3) << std::endl;

	std::cout << "-------------------------------" << std::endl;

	MACGrid->swap(*MACGrid_old);

	std::cout << u(1, 2, 3) << std::endl;
	std::cout << v(1, 2, 3) << std::endl;
	std::cout << w(1, 2, 3) << std::endl;

	std::cout << MACGrid_old->u(1, 2, 3) << std::endl;
	std::cout << MACGrid_old->v(1, 2, 3) << std::endl;
	std::cout << MACGrid_old->w(1, 2, 3) << std::endl;

	std::cout << "-------------------------------" << std::endl;

	auto MACGrid_new = std::make_shared<HSim::FaceCenterGrid3<float>>(*MACGrid_old);
	std::cout << MACGrid_new->u(1, 2, 3) << std::endl;
	std::cout << MACGrid_new->v(1, 2, 3) << std::endl;
	std::cout << MACGrid_new->w(1, 2, 3) << std::endl;

	std::cout << "-------------------------------" << std::endl;

	std::cout << isBoundary(MACGrid, 0, 0, 0) << std::endl;
	std::cout << isBoundary(MACGrid, 31, 31, 31) << std::endl;
	std::cout << isBoundary(MACGrid, 0, 1, 2) << std::endl;
	std::cout << isBoundary(MACGrid, 1, 1, 1) << std::endl;
	std::cout << isBoundary(MACGrid, 30, 30, 30) << std::endl;

	std::cout << "-------------------------------" << std::endl;

	return 0;
}