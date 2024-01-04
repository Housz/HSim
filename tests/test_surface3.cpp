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


// #define PLANE3
#ifdef PLANE3

	HSim::Plane3<float> plane;
	plane.transform.translation = {0, 1, 0};

	HSim::Vec3<float> p(-2, -2, 0);

	std::cout << plane.closestPosition(p);
	std::cout << plane.closestDistance(p);



#endif

#define BOX3
#ifdef BOX3
	HSim::Vec3<float> lower(0, 0, 0);
	HSim::Vec3<float> upper(5, 5, 5);
	HSim::Box3<float> box(lower, upper);
	
	// box.transform.translation = {2, 0, 0};

	std::cout << box.isInsideWorld({1, 1, 1}) << std::endl;

	std::cout << box.closestPosition({6, 6, 0}) << std::endl;
	std::cout << box.closestDistance({6, 6, 0}) << std::endl;
	
	std::cout << box.closestNormal({6, 2, 2}) << std::endl;

#endif

	return 0;
}