#pragma once

#include <HSim/scalar_grid3.h>

namespace HSim
{
	template <typename T>
	class VertexCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		VertexCenterScalarGrid3() {}

		VertexCenterScalarGrid3(const VertexCenterScalarGrid3<T> &vertexCenterScalarGrid3)
			: ScalarGrid3<T>(vertexCenterScalarGrid3)
		{
		}

		VertexCenterScalarGrid3(size_t x, size_t y, size_t z)
			: ScalarGrid3<T>(x, y, z)
		{
		}
		VertexCenterScalarGrid3(Vec3i resolution, Vec3<T> gridOrigin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1})
			: ScalarGrid3<T>(resolution, gridOrigin, gridSpacing)
		{
		}

		~VertexCenterScalarGrid3() {}

		void clone(std::shared_ptr<SpaceObject3<T>> &ptr) override
		{
			ptr = std::make_shared<VertexCenterScalarGrid3<T>>(*this);
		}

	public:
		Vec3i dataSize() const override
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
		Vec3<T> dataOrigin() const override
		{
			return gridOrigin;
		}

		Vec3<T> positionAt(size_t i, size_t j, size_t k) const override
		{
			return gridOrigin + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
		}

		T sample(const Vec3<T> &p) const override
		{
			size_t i = (size_t)(p.x / deltaX());
			size_t j = (size_t)(p.y / deltaY());
			size_t k = (size_t)(p.z / deltaZ());

			i = clamp(i, 0, gridResolution.x - 1);
			j = clamp(j, 0, gridResolution.y - 1);
			k = clamp(k, 0, gridResolution.z - 1);

			auto t = p - positionAt(i, j, k);

			return lerp(dataAt(i, j, k), dataAt(i + 1, j, k), dataAt(i, j + 1, k), dataAt(i + 1, j + 1, k),
						dataAt(i, j, k + 1), dataAt(i + 1, j, k + 1), dataAt(i, j + 1, k + 1), dataAt(i + 1, j + 1, k + 1),
						t.x, t.y, t.z);
		}
	};

} // namespace HSim
