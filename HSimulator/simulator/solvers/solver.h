#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

#include <config/numerical_config.h>
#include <simulator/solvers/frame.h>

namespace HSim
{
	class Solver
	{
	public:
		Solver();
		~Solver();

	public:
		virtual void update(const Frame& frame) = 0;

		void run(size_t numFrame);
	};

	using Solver_Ptr = std::shared_ptr<Solver>;

} // namespace HSim
