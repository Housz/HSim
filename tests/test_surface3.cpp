#include <HSim/sphere3.h>

int main()
{
	HSim::Sphere3<float> sphere;

	sphere.transform.setTranslation({1, 1, 0});

	std::cout << sphere.center;
	std::cout << sphere.getCenter();
	std::cout << sphere.radius << std::endl;
	

	HSim::Vec3f p = {2, 2, 0}; // world frame

	std::cout << sphere.closestPosition(p);
	std::cout << sphere.closestNormal(p);
	std::cout << sphere.closestDistance(p) << std::endl;


	return 0;
}