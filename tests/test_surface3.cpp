#include <HSim/sphere3.h>

int main()
{
	HSim::Sphere3<float> sphere;
	std::cout << sphere.center;
	std::cout << sphere.radius;

	HSim::Vec3f p = {2, 0, 0};

	std::cout << sphere.closestPosition(p);
	std::cout << sphere.closestNormal(p);
	std::cout << sphere.closestDistance(p);


	return 0;
}