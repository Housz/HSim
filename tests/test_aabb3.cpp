#include <HSim/aabb3.h>
#include <HSim/math_utils.h>

int main()
{
	auto lower = HSim::Vec3f(0, 0, 0);
	auto upper = HSim::Vec3f(0, 1, 1);

    auto aabb = std::make_shared<HSim::AABB3<float>>(lower, upper);

	std::cout << aabb->lowerCorner;
	std::cout << aabb->upperCorner;

	// for (size_t i = 0; i < 8; i++)
	// {
	// 	std::cout << aabb->corners(i);
	// }

	HSim::Vec3f p(-0.5, 0.5, 2);

	std::cout << clamp(p, aabb->lowerCorner, aabb->upperCorner);
	

	return 0;
}