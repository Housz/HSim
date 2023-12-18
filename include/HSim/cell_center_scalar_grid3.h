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

		CellCenterScalarGrid3(size_t, size_t, size_t);
		CellCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1});

	public:
		Vec3i dataSize() override;
		Vec3<T> dataOrigin() override;

	};

	template <typename T>
	CellCenterScalarGrid3<T>::CellCenterScalarGrid3(size_t x, size_t y, size_t z)
	:ScalarGrid3<T>(x, y, z)
	{
	}

	template <typename T>
	CellCenterScalarGrid3<T>::CellCenterScalarGrid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1})
	:ScalarGrid3<T>(resolution, origin, gridSpacing)
	{
	}

	template <typename T>
	Vec3i CellCenterScalarGrid3<T>::dataSize()
	{
		return _resolution;
	}

	template <typename T>
	Vec3<T> CellCenterScalarGrid3<T>::dataOrigin()
	{
		return _origin + _gridSpacing * 0.5;
	}




} // namespace HSim
