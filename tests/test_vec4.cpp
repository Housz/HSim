#include <iostream>
#include <HSim/vec4.h>

int main()
{

	HSim::Vec4f v4(1, 2, 3, 4); 
	std::cout << v4;
	std::cout << 2 * v4;
	// std::cout << HSim::vec3d({1, 2, 3});
	
	return 0;
}