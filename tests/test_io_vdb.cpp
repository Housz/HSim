// #include <HSim/io/vdbwriter.h>

// int main()
// {
// 	size_t n = 100;
// 	std::vector<float> v(n * n * n);
// 	for (size_t i = 0; i < n; i++)
// 	{
// 		for (size_t j = 0; j < n; j++)
// 		{
// 			for (size_t k = 0; k < n; k++)
// 			{
// 				v[i * n * n + j * n + k] = 0;
// 			}
// 		}
// 	}
// 	VDBWriter<float> vdbWriter;

// 	vdbWriter.addGrid("density", v, n, n, n);
// 	vdbWriter.writeVDB("test.vdb");

// 	return 0;
// }

#include <HSim/io/writevdb.h>

int main()
{
	size_t n = 100;
	std::vector<float> v(n * n * n, 1);
	// for (size_t i = 0; i < n; i++)
	// {
	// 	for (size_t j = 0; j < n; j++)
	// 	{
	// 		for (size_t k = 0; k < n; k++)
	// 		{
	// 			v[i * n * n + j * n + k] = 10;
	// 		}
	// 	}
	// }

	VDBWriter writer;

	// writer.addGrid<float, 1>("density", v.data(), n, n, n);
	// writer.addGrid<float, 1>("density1", v.data(), n, n, n);
	writer.addGrid<float, 1>("temperature", v.data(), n, n, n);
	// writer.addGrid<float, 1>("my", v.data(), n, n, n);
	writer.write("writer.vdb");

	return 0;
}