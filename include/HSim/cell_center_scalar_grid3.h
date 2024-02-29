#pragma once 

#include <HSim/scalar_grid3.h>

namespace HSim
{

	template <typename T>
	class CellCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		CellCenterScalarGrid3() {}
		~CellCenterScalarGrid3() {}

		CellCenterScalarGrid3(size_t x, size_t y, size_t z) 
		: Grid3(x, y, z) {}

		CellCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
		: Grid3(resolution, origin, gridSpacing) {}

	public:
		Vec3i dataSize() override
		{
			return _resolution;
		}

		Vec3<T> dataOrigin() override
		{
			return _origin + _gridSpacing * 0.5;
		}
	
	// for rendering
	public:
		void drawData()
		{
			std::cout << "drawData cell_center_scalar_grid" << std::endl;
		}

	};


} // namespace HSim
