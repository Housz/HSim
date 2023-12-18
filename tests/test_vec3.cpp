#include <iostream>
#include <HSim/vec3.h>

int main()
{

	HSim::Vec3d v3(1, 2, 3); 
	v3 = {2, 3, 4};
	std::cout << v3;
	std::cout << 2 * v3;
	// std::cout << HSim::vec3d({1, 2, 3});
	
	return 0;
}