#include <HSim/triangle_mesh3.h>

#include <HSim/triangle3.h>

int main()
{
	// test triangle3

	HSim::Triangle3<float> tri;

	tri.points[0] = {0, 0, 0};
	tri.points[1] = {1, 0, 0};
	tri.points[2] = {0, 1., 0};

	std::cout << tri.area() << std::endl;
	std::cout << tri.faceNormal();

	HSim::Vec3f p_local({1.5, 0.5, -0.5});
	tri.closestDistanceLocal(p_local);

	return 0;
}