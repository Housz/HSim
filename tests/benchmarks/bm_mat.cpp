#include <iostream>
#include <vector>

#include <HSim/mat.h>

#include <benchmark/benchmark.h>
#include <Eigen/Dense>


size_t n = 10000;

static void eigen_dense_mul(benchmark::State& state)
{
	for (auto _ : state) {
		auto m1 = Eigen::MatrixXd::Random(n, n);
		auto m2= Eigen::MatrixXd::Random(n, n);
		// auto mm = Eigen::MatrixXd::Zero(n, n);
		auto mm = m1*m2;
		// std::cout << mm(0,1);
	}
}

static void hsim_mul(benchmark::State& state)
{
	for (auto _ : state) {
		HSim::Mat<float, 1000, 1000> m1(1), m2(1);
    	auto mm = m1*m2;
	}
}

static void only_allocate(benchmark::State& state)
{
	for (auto _ : state) {
		std::vector<std::vector<float>>(n, std::vector<float>(n));
	}
}

static void only_allocate_nxn(benchmark::State& state)
{
	for (auto _ : state) {
		std::vector<float>(n*n);
	}
}

BENCHMARK(eigen_dense_mul);
BENCHMARK(only_allocate);
BENCHMARK(only_allocate_nxn);
BENCHMARK(hsim_mul);

BENCHMARK_MAIN();
