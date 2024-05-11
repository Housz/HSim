#pragma once

#include <HSim/scalar_grid3.h>

namespace HSim
{
	template <typename T>
	class VertexCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		VertexCenterScalarGrid3() {}

		VertexCenterScalarGrid3(const VertexCenterScalarGrid3<T>& vertexCenterScalarGrid3) 
			: ScalarGrid3<T>(vertexCenterScalarGrid3)
		{
		}

		VertexCenterScalarGrid3(size_t x, size_t y, size_t z)
			: ScalarGrid3<T>(x, y, z)
		{
		}
		VertexCenterScalarGrid3(Vec3i resolution, Vec3<T> origin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1})
			: ScalarGrid3<T>(resolution, origin, gridSpacing)
		{
		}

		~VertexCenterScalarGrid3() {}

		void clone(std::shared_ptr<SpaceObject3<T>>& ptr) override
		{
			ptr = std::make_shared<VertexCenterScalarGrid3<T>>(*this);
		}

	public:
		Vec3i dataSize() override
		{
			if (gridResolution.isZero())
			{
				return Vec3i();
			}
			else
			{
				return gridResolution + Vec3i(1, 1, 1);
			}
		}
		Vec3<T> dataOrigin() override
		{
			return girdOrigin;
		}
	};

} // namespace HSim
