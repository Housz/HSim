#include <iostream>
#include <HSim/vec3.h>

int main()
{

	HSim::Vec3d v(1, 2, 3); 
	v = {2, 3, 4};
	std::cout << v;
	std::cout << 2 * v;
	// std::cout << HSim::vec3d({1, 2, 3});

	HSim::Vec3d v1(1, 1, 1);
	HSim::Vec3d v2(-1, -1, -1);

	std::cout << v1.distanceTo(v2);
	
	return 0;
}