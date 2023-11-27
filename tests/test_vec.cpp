#include <iostream>
#include <HSim/vec.h>
#include <HSim/parallel.h>

int main()
{
	// size_t n = 1 << 24;
	size_t n = 1 << 7;
	HSim::Vecf v1(n), v2(n);
	v1.set(1);
	v2.set(2);
	// std::cout << v1 << v2;


	auto callback = [&](size_t i){
		v1[i] += 1;
	};

	// HSim::parallelFor(size_t(0), size_t(v1.size()), callback);
	HSim::parallelFor(size_t(0), v1.size(), [&](size_t i){
		v1[i] += 1;
	});
	std::cout << v1;
	v1.forEachParallel(callback);
	std::cout << v1;

	v1.set(v2);
	std::cout << v1;

	HSim::Vecd vd(v1);
	std::cout << vd;

	std::cout << v1.dot(v2) << std::endl;
	std::cout << v1.sum() << std::endl;
	HSim::parallelFor(size_t(0), v1.size(), [&](size_t i){
		v1[i] = i;
	});
	std::cout << v1.avg() << std::endl;
	std::cout << v1.min() << std::endl;



	return 0;
}