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
	std::cout << v1;
	std::cout << v1.avg() << std::endl;
	std::cout << v1.min() << std::endl;
	std::cout << v1.max() << std::endl;

	auto v_add = v1.add(v1);
	std::cout << v_add;

	v1.add_self(v1);
	std::cout << v1;

	HSim::Vecf v_op = v2;
	std::cout << (v2 == v_op) << std::endl;;
	v_op += 2;
	std::cout << v_op;
	v_op -= 1;
	std::cout << v_op;
	v_op *= 3.14;
	std::cout << v_op;
	v_op /= 3.14;
	std::cout << v_op;
	std::cout << (v2 == v_op) << std::endl;;
	v_op += v2;
	std::cout << v_op;
	v_op -= v2;
	std::cout << v_op;
	v_op *= v2;
	std::cout << v_op;
	v_op /= v2;
	std::cout << v_op;

	HSim::Vecf v_norm {3, 4};
	std::cout << v_norm.length() << std::endl;
	std::cout << v_norm.norm() << std::endl;
	v_norm.normalize();
	std::cout << v_norm << std::endl;



	return 0;
}