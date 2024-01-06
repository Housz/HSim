#pragma once

/**
 * reference: https://github.com/parallel101/course/tree/master/09/
 */

#include <string>
#include <array>
#include <vector>
#include <memory>
#include <openvdb/openvdb.h>
#include <openvdb/tools/Dense.h>

template <typename T>
class VDBWriter
{
	// void addGrid(name, data, size x, size y, size z)
	// void write(path)
	openvdb::GridPtrVec grids;

public:
	void addGrid(const std::string &name, std::vector<T> &source, size_t size_x, size_t size_y, size_t size_z)
	{
		// Let the origin be in the center of the grid.
		int32_t minx = -(int32_t)size_x / 2;
		int32_t miny = -(int32_t)size_y / 2;
		int32_t minz = -(int32_t)size_z / 2;

		// size_t pitchx = sizeof(T) * N;
        // size_t pitchy = pitchx * sizex;
        // size_t pitchz = pitchy * sizey;

		// type: openvdb::FloatGrid
		openvdb::tools::Dense<openvdb::FloatGrid::ValueType> dense(openvdb::Coord(size_x, size_y, size_z), openvdb::Coord(minx, miny, minz));

		// for (size_t z = 0; z < size_z; z++)
		// {
		// 	for (size_t y = 0; y < size_y; y++)
		// 	{
		// 		for (size_t z = 0; z < size_z; z++)
		// 		{
		// 			auto ptr = reinterpret_cast<T const *>(reinterpret_cast<char const *>(source.data()) + pitchx * x + pitchy * y + pitchz * z);

		// 			dense.setValue(x, y, z, source[]);
		// 		}
				
		// 	}
			
		// }

		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				for (size_t k = 0; k < size_z; k++)
				{
					dense.setValue(i, j, k, source[i*size_x*size_y + j*size_x + k]);
					// std::cout << source[i*size_x*size_y + j*size_x + k];
				}
				
			}
		}


		auto grid = openvdb::FloatGrid::create();
		openvdb::FloatGrid::ValueType tolerance{0};
		openvdb::tools::copyFromDense(dense, grid->tree(), tolerance);

		openvdb::MetaMap &meta = *grid;
		meta.insertMeta(openvdb::Name("name"), openvdb::TypedMetadata<std::string>(name));

		openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
		openvdb::Coord xyz(10, 20, 30);
		std::cout << accessor.getValue(xyz) << std::endl;

		grids.push_back(grid);
	}

	void writeVDB(const std::string &path)
	{
		openvdb::io::File(path).write(grids);
	}
};