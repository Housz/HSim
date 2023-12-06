#include <iostream>
#include <fstream>
#include <cstdlib>

#include <HSim/scalar_field3.h>

using namespace HSim;

template <typename T, size_t X, size_t Y, size_t Z>
class MyScalarField3 : public ScalarField3<T, X, Y, Z>
{
public:
	T sample(const Vec3i &pos) override
	{
		const float scalar = 0.1;
		assert(pos.x < X && pos.y < Y && pos.z < Z);
		return std::sin(pos.x * scalar) * std::sin(pos.y * scalar) * std::sin(pos.z * scalar);
	}

	T sample_scale(const Vec3i &pos, const T scalar)
	{
		assert(pos.x < X && pos.y < Y);
		return std::sin(pos.x * scalar) * std::sin(pos.y * scalar) * std::sin(pos.z * scalar);
	}

	// to do
	Vec3<T> gradient(const Vec3i &pos) override
	{
		Vec3<T> grad;
		return grad;
	}

	// to do
	T laplacian(const Vec3i &pos) override
	{
		return 0;
	}
};

int main(int argc, char *argv[])
{
	float scalar = 1;
	if (argc == 2)
	{
		scalar = std::atof(argv[1]);
	}

	const size_t X = 100, Y = 100, Z = 100;
	MyScalarField3<float, X, Y, Z> sf3;

	// out
	std::ofstream fs;
	std::ofstream outFile("a.txt");
	if (outFile.is_open())
	{

		for (size_t i = 0; i < X; i++)
		{
			for (size_t j = 0; j < Y; j++)
			{
				for(size_t k = 0; k < Z; k++)
				{
					auto s = sf3.sample_scale(Vec3i(i, j, k), scalar);

					outFile << s << " ";
				}
				outFile << std::endl;
			}
			outFile << std::endl;
		}

		outFile.close();
		std::cout << "out done: a.txt" << std::endl;
	}
	else
	{
		std::cerr << "out error" << std::endl;
	}
}