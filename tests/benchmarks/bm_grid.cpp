/*

----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
for_each_serial  527002475 ns    253906250 ns            4
for_each_para     89147750 ns     57812500 ns           10

*/
#include <iostream>
#include <vector>
#include <cmath>

#include <HSim/face_center_grid3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/vertex_center_scalar_grid3.h>

#include <benchmark/benchmark.h>

HSim::Size3 gridResolution = {512, 512, 512};
auto MACGrid = std::make_shared<HSim::FaceCenterGrid3<float>>(gridResolution);
auto &u = MACGrid->dataU();
auto &v = MACGrid->dataV();
auto &w = MACGrid->dataW();

auto fillCallbackU = [&](size_t i, size_t j, size_t k)
{
	u(i, j, k) = i;
};

auto eachCallback = [&](size_t i, size_t j, size_t k)
{
	if (i <= 0 || j <= 0 || k <= 0 ||
		i >= MACGrid->sizeX() - 1 || j >= MACGrid->sizeY() - 1 || k >= MACGrid->sizeZ() - 1)
	{
		return;
	}

	auto div = u(i + 1, j, k) +
			   -u(i, j, k) +
			   v(i, j + 1, k) +
			   -v(i, j, k) +
			   w(i, j, k + 1) +
			   -w(i, j, k);

	div /= 6;

	u(i + 1, j, k) -= div;
	u(i, j, k) += div;
	v(i, j + 1, k) -= div;
	v(i, j, k) += div;
	w(i, j, k + 1) -= div;
	w(i, j, k) += div;
};

// static void for_face_para(benchmark::State & state)
// {
// 	for (auto _ : state)
// 	{
// 		u.parallelForEachCell(fillCallbackU);
// 	}
// }

static void for_each_serial(benchmark::State &state)
{
	for (auto _ : state)
	{
		MACGrid->forEachCell(eachCallback);
	}
}

static void for_each_para(benchmark::State &state)
{
	for (auto _ : state)
	{
		MACGrid->parallelForEachCell(eachCallback);
	}
}

BENCHMARK(for_each_serial);
BENCHMARK(for_each_para);

BENCHMARK_MAIN();
