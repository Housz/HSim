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
		: ScalarGrid3<T>(x, y, z) {}

		CellCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
		: ScalarGrid3<T>(resolution, origin, gridSpacing) {}

	public:
		Vec3i dataSize() override
		{
			return _gridResolution;
		}

		Vec3<T> dataOrigin() override
		{
			return _girdOrigin + _gridSpacing * 0.5;
		}
	
	// for rendering
	public:
		// draw boundary
		void drawBoundary() override 
		{
			std::cout << "drawBoundary cell_center_scalar_grid3" << std::endl;
			this->aabb->draw();
		}

		
		// draw grid
		void drawGrid() override
		{
			std::cout << "drawGrid cell_center_scalar_grid3" << std::endl;
		}

		// draw data
		// put all data to a VBO, glDrawArrays() 
		void drawData() override
		{
			std::cout << "drawData cell_center_scalar_grid3" << std::endl;
		}

	};


} // namespace HSim
