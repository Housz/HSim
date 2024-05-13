#pragma once

#include <HSim/common.h>
#include <HSim/transform3.h>

#include <config/configs.h>
#include <simulator/simulator_frame.h>

namespace HSim
{
	class Solver
	{
	public:
		Solver();
		~Solver();

	public:
		virtual void update(const SimFrame& frame) = 0;

		virtual void writeRendererBuffer() = 0;

		void run(size_t numFrame);
	};

	using Solver_Ptr = std::shared_ptr<Solver>;

} // namespace HSim
