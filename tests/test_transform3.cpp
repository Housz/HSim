#include <HSim/transform3.h>

int main()
{
	HSim::Vec3f axis(0, 0, 1);
	float angle = PI_QUARTER;
	HSim::Quaternionf orientation(axis, angle);

	HSim::Vec3f translation(0, 0, 1);
	HSim::Transform3<float> t(orientation, translation);

	std::cout << t.getOrientationMatrix();
	std::cout << t.getTransformMatrix();

	

	return 0;
}