#include <iostream>
#include <HSim/vec.h>

int main()
{
	size_t n = 1 << 24;
	HSim::Vecf v1(n), v2(n);
	v1.set(1);
	v2.set(2);
	// std::cout << v1 << v2;

	std::cout << v1.dot(v2) << std::endl;

	return 0;
}