#include <HSim/aabb3.h>

int main()
{
	auto lower = HSim::Vec3f(0, 0, 0);
	auto upper = HSim::Vec3f(1, 1, 1);

    auto aabb = std::make_shared<HSim::AABB3<float>>(lower, upper);

	std::cout << aabb->lowerCorner;
	std::cout << aabb->upperCorner;

	return 0;
}