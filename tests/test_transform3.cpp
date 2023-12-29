#include <HSim/transform3.h>

int main()
{
// #define SET
#ifdef SET
	HSim::Vec3f axis(0, 0, 1);
	float angle = PI_QUARTER;
	HSim::Quaternionf orientation(axis, angle);

	HSim::Vec3f translation(0, 0, 1);
	HSim::Transform3<float> t(orientation, translation);

	std::cout << t.getOrientationMatrix();
	std::cout << t.getTransformMatrix();
#endif 

#define TRANS
#ifdef TRANS
	HSim::Vec3f axis(0, 0, 1);
	float angle = PI_HALF;
	HSim::Quaternionf orientation(axis, angle);

	HSim::Vec3f translation(0, 0, 1);
	HSim::Transform3<float> t(orientation, translation);

	HSim::Vec3f v1({1, 0, 0});
	std::cout << t.mul(v1); // (0, 1, 1)

	HSim::Vec3f v2({-1, 1, 1});
	std::cout << t.inverse_mul(v2); // (1, 1, 0)


#endif 




	

	return 0;
}