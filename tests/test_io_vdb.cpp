// #define vdbwriter
#ifdef vdbwriter
#include <HSim/io/vdbwriter.h>

int main()
{
	size_t n = 1 << 5;
	std::vector<float> v(n * n * n, 1.0);
	// for (size_t i = 0; i < n; i++)
	// {
	// 	for (size_t j = 0; j < n; j++)
	// 	{
	// 		for (size_t k = 0; k < n; k++)
	// 		{
	// 			if (i > n/2)
	// 			{
	// 				v[i * n * n + j * n + k] = 1.0;
	// 			}
	// 		}
	// 	}
	// }
	VDBWriter<float> vdbWriter;

	vdbWriter.addGrid("density", v, n, n, n);
	vdbWriter.writeVDB("test.vdb");

	return 0;
}
#endif

// #define writevdb
#ifdef writevdb

#include <HSim/io/writevdb.h>

int main()
{
	size_t n = 100;
	std::vector<float> v(n * n * n);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			for (size_t k = 0; k < n; k++)
			{
				if (i > n / 2)
				{
					// v[i * n * n + j * n + k] = 10;
				}
			}
		}
	}

	VDBWriter writer;

	// writer.addGrid<float, 1>("density", v.data(), n, n, n);
	// writer.addGrid<float, 1>("density1", v.data(), n, n, n);
	writer.addGrid<float, 1>("temperature", v.data(), n, n, n);
	// writer.addGrid<float, 1>("my", v.data(), n, n, n);
	writer.write("writer.vdb");

	return 0;
}

#endif

#include <openvdb/openvdb.h>

int main()
{
	size_t n = 1 << 9;

	openvdb::initialize();

	openvdb::FloatGrid::Ptr grid =
		openvdb::FloatGrid::create(/*background value=*/);

	openvdb::FloatGrid::Accessor accessor = grid->getAccessor();

	for (uint32_t i = 0; i < n; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			for (uint32_t k = 0; k < n; k++)
			{
				if (i > n / 2 && j > n / 2)
				{
					openvdb::Coord ijk(i, j, k);
					accessor.setValue(ijk, 2.0);
				}
			}
		}
	}

	grid->setName("mygrid");

	openvdb::GridPtrVec grids;
	grids.push_back(grid);

	openvdb::io::File file("mygrids.vdb");
	file.write(grids);
	file.close();

	return 0;
}