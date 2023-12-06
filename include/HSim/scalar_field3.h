#pragma once

#include <HSim/common.h>
#include <HSim/field3.h>

namespace HSim
{

	template <typename T, size_t X, size_t Y, size_t Z>
    class ScalarField3 : public Field3<T, X, Y, Z>
	{
	
	public:
		ScalarField3() {}
		~ScalarField3() {}

	public:
		virtual T sample(const Vec3i& pos) = 0;
		virtual Vec3<T> gradient(const Vec3i& pos) = 0;
		virtual T laplacian(const Vec3i& pos) = 0;

	};

} // namespace HSim
