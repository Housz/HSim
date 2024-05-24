#pragma once

#include <HSim/array3.h>

namespace HSim
{
	// symmetry. right == left, up == down, front == back
	struct PossionMatrixRow3
	{

		double center = 0.;

		double right = 0.;

		double up = 0.;

		double front = 0.;
	};

	using PossionMatrix3 = Array3<PossionMatrixRow3>;
	using PossionVector3 = Array3<double>;

	// Ax = b
	struct LinearSystem3
	{
		PossionMatrix3 A;

		PossionVector3 x;

		PossionVector3 b;

		void resize(size_t x_, size_t y_, size_t z_)
		{
			A.resize(x_, y_, z_);
			x.resize(x_, y_, z_);
			b.resize(x_, y_, z_);
		}

		void resize(Size3 size)
		{
			A.resize(size);
			x.resize(size);
			b.resize(size);
		}

		void clear()
		{
			A.clear();
			x.clear();
			b.clear();
		}
	};

} // namespace HSim
