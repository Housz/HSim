#include <iostream>
#include <HSim/vec4.h>

int main()
{

	HSim::Vec4f v(1, 2, 3, 4); 
	std::cout << v;
	std::cout << 2 * v;

	HSim::Vec4f v1({1});
	HSim::Vec4f v2({1, 2});
	HSim::Vec4f v3 = {1, 2, 3};
	HSim::Vec4f v4 = {1, 2, 3, 4};
	HSim::Vec4f v5 = {1, 2, 3, 4, 5};

	std::cout << v1; std::cout << v2; std::cout << v3; std::cout << v4; std::cout << v5; 

	v4.set({-1, -2});
	std::cout << v4;
	
	return 0;
}