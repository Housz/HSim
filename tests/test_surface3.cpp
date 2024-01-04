#include <HSim/sphere3.h>
#include <HSim/plane3.h>
#include <HSim/box3.h>
#include <HSim/aabb3.h>
#include <memory>


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

// #define boundingbox
#ifdef boundingbox

	HSim::AABB3<float> aabb1({0, 0, 0}, {2, 2, 2});
	HSim::AABB3<float> aabb2({1, 1, 1}, {3, 3, 3});
	HSim::AABB3<float> aabb3({-1, -1, -1}, {0, 0, 0});


	std::cout << aabb1.isOverlap(aabb2) << std::endl;
	std::cout << aabb2.isOverlap(aabb3) << std::endl;
	std::cout << aabb1.isInside({1, 1, 1}) << std::endl;

	HSim::Vec3<float> lower(0, 0, 0);
	HSim::Vec3<float> upper(5, 5, 5);
	// HSim::AABB3Ptr<float> a = std::make_shared<HSim::AABB3<float>>({0, 0, 0}, {2, 2, 2}); error
	HSim::AABB3_Ptr<float> a = std::make_shared<HSim::AABB3<float>>(lower, upper);
	std::cout << a->upperCorner;

#endif 

	return 0;
}