#include <HSim/sphere3.h>
#include <HSim/plane3.h>
#include <HSim/box3.h>


int main()
{

// #define SPHERE3
#ifdef SPHERE3
	HSim::Sphere3<float> sphere;

	sphere.transform.setTranslation({1, 1, 0});

	std::cout << sphere.center;
	std::cout << sphere.getCenter();
	std::cout << sphere.radius << std::endl;
	

	HSim::Vec3f p = {2, 2, 0}; // world frame

	std::cout << sphere.closestPosition(p);
	std::cout << sphere.closestNormal(p);
	std::cout << sphere.closestDistance(p) << std::endl;
#endif


#define PLANE3
#ifdef PLANE3



#endif

// #define BOX3
#ifdef BOX3

#endif

	return 0;
}