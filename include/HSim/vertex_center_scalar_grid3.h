#pragma once 

#include <HSim/scalar_grid3.h>

namespace HSim
{
	template <typename T>
	class VertexCenterScalarGrid3 : public ScalarGrid3<T>
	{
	public:
		VertexCenterScalarGrid3() {}
		~VertexCenterScalarGrid3() {}

		VertexCenterScalarGrid3(size_t, size_t, size_t);
		VertexCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1});

	public:
		Vec3i dataSize() override;
		Vec3<T> dataOrigin() override;

	};

	template <typename T>
	VertexCenterScalarGrid3<T>::VertexCenterScalarGrid3(size_t x, size_t y, size_t z)
	:ScalarGrid3<T>(x, y, z)
	{
	}

	template <typename T>
	VertexCenterScalarGrid3<T>::VertexCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
	:ScalarGrid3<T>(resolution, origin, gridSpacing)
	{
	}

	template <typename T>
	Vec3i VertexCenterScalarGrid3<T>::dataSize()
	{
		if (_gridResolution.isZero())
		{
			return Vec3i();
		}
		else
		{
			return _gridResolution + Vec3i(1, 1, 1);
		}
	}

	template <typename T>
	Vec3<T> VertexCenterScalarGrid3<T>::dataOrigin()
	{
		return _girdOrigin;
	}





} // namespace HSim
