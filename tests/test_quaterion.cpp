#include <HSim/quaternion.h>

int main()
{
	HSim::Quaternionf q1(0.92388, 0, 0, 0.382683); // axis (0, 0, 1), angle PI/4	
	std::cout << q1.axis();
	std::cout << q1.angle() << std::endl;

	std::cout << q1.toMatrix();

	q1.rotate(PI_QUARTER); // axis (0, 0, 1), angle PI/2	
	std::cout << q1.axis();
	std::cout << q1.angle() << std::endl;

	HSim::Vec3f v(1, 1, 0);
	std::cout << q1*v; // (-1, 1, 0)

	HSim::Quaternionf q2;
	q2.set({0, 1, 0}, PI_HALF); // axis (0, 1, 0), angle PI/4	
	std::cout << q2*q1*v; // (0, 1, 1);

	return 0;
}