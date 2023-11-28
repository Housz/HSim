#include <iostream>
#include <vector>
#include <cmath>

#include <HSim/vec.h>

#include <benchmark/benchmark.h>


size_t n = 1 << 27;

HSim::Vecf v1(n);

static void vec_sum(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    v1.sum();
  }
}

static void for_sum(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
	float sum = 0;
    for (size_t i = 0; i < n; i++)
	{
		sum += v1[i];
	}
	
  }
}
// Register the function as a benchmark
BENCHMARK(vec_sum);
BENCHMARK(for_sum);



// Run the benchmark
BENCHMARK_MAIN();
