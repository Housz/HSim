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

	inline void linearSystem3Residual(LinearSystem3 &system, PossionVector3 &residual)
	{
		auto &A = system.A;
		auto &x = system.x;
		auto &b = system.b;

		auto size = A.size;

		residual.parallelForEachCell(
			[&](size_t i, size_t j, size_t k)
			{
				residual(i, j, k) =
					b(i, j, k) -
					(A(i, j, k).center * x(i, j, k) +
					((i > 0) ? A(i, j, k).right * x(i - 1, j, k) : 0.0) +
					((i < size.x - 1) ? A(i, j, k).right * x(i + 1, j, k) : 0.0) +
					((j > 0) ? A(i, j, k).up * x(i, j - 1, k) : 0.0) +
					((j < size.y - 1) ? A(i, j, k).up * x(i, j + 1, k) : 0.0) +
					((k > 0) ? A(i, j, k).front * x(i, j, k - 1) : 0.0) +
					((k < size.z - 1) ? A(i, j, k).front * x(i, j, k + 1) : 0.0));
			});
	}

	inline double possionVector3L2Norm(const PossionVector3 &v)
	{
		double sum = parallelReduce(
			v._data.begin(), v._data.end(),

			0.,

			[&](tbb::blocked_range<size_t> r, double local_sum)
			{
				for (size_t i = r.begin(); i < r.end(); i++)
				{
					local_sum += v._data[i] * v._data[i];
				}
				return local_sum;
			},

			std::plus<double>());

		return std::sqrt(sum);
	}

} // namespace HSim
