#include <HSim/math_utils.h>


int main()
{
	double n0 = -1, n1 = 0;
	double t = .3;

	std::cout << HSim::lerp(n0, n1, t) << "\n";

	std::cout << HSim::lerp(0., 1., 0., 1., .3, .5) << "\n";

	std::cout << HSim::lerp(0., 0., 0., 0., 1., 1., 1., 1., .5, .5, .5) << "\n";
}