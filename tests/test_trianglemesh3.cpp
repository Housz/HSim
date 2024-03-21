#include <HSim/triangle_mesh3.h>

#include <HSim/triangle3.h>

int main()
{
	// test triangle3

	HSim::Triangle3<float> tri;

	tri.points[0] = {0, 0, 0};
	tri.points[1] = {1, 0, 0};
	tri.points[2] = {0, 1., 1};

	tri.normals[0] = {0, 0, 1};
	tri.normals[1] = {0, 0, 1};
	tri.normals[2] = {0, 0, 1};

	// std::cout << tri.area() << std::endl;
	// std::cout << tri.faceNormal();

	HSim::Vec3f p_local({.5, 0, 0});
	// std::cout << tri.closestDistanceLocal(p_local);
	// std::cout << tri.closestNormalLocal(p_local);
	// std::cout << tri.barycentricCoords(p_local);

	std::cout << tri.AABBLocal().lowerCorner;
	std::cout << tri.AABBLocal().upperCorner;

	return 0;
}